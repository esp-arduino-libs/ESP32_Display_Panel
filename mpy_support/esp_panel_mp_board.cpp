/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "utils/esp_panel_utils_log.h"
#include "utils/esp_panel_utils_cxx.hpp"
#include "board/esp_panel_board.hpp"
#include "esp_panel_mp_types.h"
#include "esp_panel_mp_board.h"

namespace esp_panel::board {

/**
 * MicroPython Wrappers
 */

// Soft-reset guard
//
// MicroPython soft resets wipe the Python heap and re-run main.py, but C++
// static variables in the firmware survive. Without this guard, every soft
// reset causes a new Board() + begin(), which re-runs the LCD vendor init
// sequence (including a display-off command) and corrupts the display.
//
// g_board_instance: weak_ptr to the active Board so make_new can reuse it
//   across soft resets rather than creating a new C++ Board object.
//   A weak_ptr is used (not shared_ptr) so the Board is still destroyed on
//   hard reset / power cycle when no MP_Board object holds a reference.
//
// g_board_began: once begin() has run once per hard boot, skip it on
//   subsequent calls (e.g., after soft reset re-imports main.py).
static std::weak_ptr<Board> g_board_instance;
static bool g_board_began = false;

// Object
struct MP_Board {
    mp_obj_base_t base;
    std::shared_ptr<Board> board = nullptr;
};

static mp_obj_t board_del(mp_obj_t self_in)
{
    MP_Board *self = static_cast<MP_Board *>(MP_OBJ_TO_PTR(self_in));

    self->board = nullptr;

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1_CXX(board_del_func_obj, board_del);

static mp_obj_t board_init(mp_obj_t self_in)
{
    MP_Board *self = static_cast<MP_Board *>(MP_OBJ_TO_PTR(self_in));

    return mp_obj_new_bool(self->board->init());
}
static MP_DEFINE_CONST_FUN_OBJ_1_CXX(board_init_func_obj, board_init);

static mp_obj_t board_begin(mp_obj_t self_in)
{
    MP_Board *self = static_cast<MP_Board *>(MP_OBJ_TO_PTR(self_in));

    // Check static flag to prevent double-begin across soft resets
    if (g_board_began) {
        return mp_const_true;  // Already began in this boot cycle
    }
    bool result = self->board->begin();
    if (result) {
        g_board_began = true;
    }
    return mp_obj_new_bool(result);
}
static MP_DEFINE_CONST_FUN_OBJ_1_CXX(board_begin_func_obj, board_begin);

static mp_obj_t board_deinit(mp_obj_t self_in)
{
    MP_Board *self = static_cast<MP_Board *>(MP_OBJ_TO_PTR(self_in));

    return mp_obj_new_bool(self->board->del());
}
static MP_DEFINE_CONST_FUN_OBJ_1_CXX(board_deinit_func_obj, board_deinit);

static mp_obj_t board_color_bar_test(mp_obj_t self_in)
{
    MP_Board *self = static_cast<MP_Board *>(MP_OBJ_TO_PTR(self_in));

    return mp_obj_new_bool(self->board->getLCD()->colorBarTest());
}
static MP_DEFINE_CONST_FUN_OBJ_1_CXX(board_color_bar_test_func_obj, board_color_bar_test);

static mp_obj_t board_get_width(mp_obj_t self_in)
{
    MP_Board *self = static_cast<MP_Board *>(MP_OBJ_TO_PTR(self_in));

    return mp_obj_new_int(self->board->getLCD()->getFrameWidth());
}
static MP_DEFINE_CONST_FUN_OBJ_1_CXX(board_get_width_func_obj, board_get_width);

static mp_obj_t board_get_height(mp_obj_t self_in)
{
    MP_Board *self = static_cast<MP_Board *>(MP_OBJ_TO_PTR(self_in));

    return mp_obj_new_int(self->board->getLCD()->getFrameHeight());
}
static MP_DEFINE_CONST_FUN_OBJ_1_CXX(board_get_height_func_obj, board_get_height);

// draw_bitmap(x, y, w, h, buf) — buf must be a bytes/bytearray of w*h*2 bytes (RGB565)
static mp_obj_t board_draw_bitmap(size_t n_args, const mp_obj_t *args)
{
    MP_Board *self = static_cast<MP_Board *>(MP_OBJ_TO_PTR(args[0]));
    int x = mp_obj_get_int(args[1]);
    int y = mp_obj_get_int(args[2]);
    int w = mp_obj_get_int(args[3]);
    int h = mp_obj_get_int(args[4]);
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[5], &bufinfo, MP_BUFFER_READ);

    auto lcd = self->board->getLCD();
    const uint8_t *src = static_cast<const uint8_t *>(bufinfo.buf);
    int bytes_per_px = lcd->getFrameColorBits() / 8;
    int chunk_rows = 30;
    int stride = w * bytes_per_px;

    // For QSPI bus, byte-swap each 16-bit pixel to match SPI_SWAP_DATA_TX used internally
    bool is_qspi = (lcd->getBus()->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_QSPI);

    // Allocate a chunk buffer for swapped data
    int chunk_bytes = chunk_rows * stride;
    std::vector<uint8_t> chunk_buf(chunk_bytes);

    bool ok = true;
    for (int row = 0; row < h && ok; row += chunk_rows) {
        int rows = (row + chunk_rows <= h) ? chunk_rows : (h - row);
        int nbytes = rows * stride;
        const uint8_t *chunk_src = src + row * stride;

        if (is_qspi && bytes_per_px == 2) {
            for (int i = 0; i < nbytes; i += 2) {
                chunk_buf[i]     = chunk_src[i + 1];
                chunk_buf[i + 1] = chunk_src[i];
            }
            ok = lcd->drawBitmap(x, y + row, w, rows, chunk_buf.data(), -1);
        } else {
            ok = lcd->drawBitmap(x, y + row, w, rows, chunk_src, -1);
        }
    }
    return mp_obj_new_bool(ok);
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(board_draw_bitmap_func_obj, 6, 6, board_draw_bitmap);

// read_touch() -> list of (x, y, strength) tuples, or empty list if no touch
static mp_obj_t board_read_touch(mp_obj_t self_in)
{
    MP_Board *self = static_cast<MP_Board *>(MP_OBJ_TO_PTR(self_in));

    auto *touch = self->board->getTouch();
    if (touch == nullptr || !touch->isOverState(drivers::Touch::State::BEGIN)) {
        return mp_obj_new_list(0, nullptr);
    }

    drivers::TouchPoint points[5];
    int n = touch->readPoints(points, 5, 0);

    mp_obj_t list = mp_obj_new_list(0, nullptr);
    for (int i = 0; i < n; i++) {
        mp_obj_t tuple[3] = {
            mp_obj_new_int(points[i].x),
            mp_obj_new_int(points[i].y),
            mp_obj_new_int(points[i].strength),
        };
        mp_obj_list_append(list, mp_obj_new_tuple(3, tuple));
    }
    return list;
}
static MP_DEFINE_CONST_FUN_OBJ_1_CXX(board_read_touch_func_obj, board_read_touch);

// Local dict
static const mp_rom_map_elem_t locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR__del__), MP_ROM_PTR(&board_del_func_obj) },
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&board_init_func_obj) },
    { MP_ROM_QSTR(MP_QSTR_begin), MP_ROM_PTR(&board_begin_func_obj) },
    { MP_ROM_QSTR(MP_QSTR_deinit), MP_ROM_PTR(&board_deinit_func_obj) },
    { MP_ROM_QSTR(MP_QSTR_color_bar_test), MP_ROM_PTR(&board_color_bar_test_func_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_width), MP_ROM_PTR(&board_get_width_func_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_height), MP_ROM_PTR(&board_get_height_func_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_bitmap), MP_ROM_PTR(&board_draw_bitmap_func_obj) },
    { MP_ROM_QSTR(MP_QSTR_read_touch), MP_ROM_PTR(&board_read_touch_func_obj) },
};
static MP_DEFINE_CONST_DICT(board_locals_dict, locals_dict_table);

// Constructor
static mp_obj_t make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args)
{
    MP_Board *self = mp_obj_malloc(MP_Board, &esp_panel_mp_board_type);

    // Try to reuse existing Board instance from previous session (soft reset)
    self->board = g_board_instance.lock();
    if (!self->board) {
        // No existing instance, create new one
        self->board = utils::make_shared<Board>();
        g_board_instance = self->board;  // Store for future reuse
    }

    return MP_OBJ_FROM_PTR(self);
}

// Print
static void print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind)
{
    mp_printf(print, "Board");
}

} // namespace esp_panel

// Type
MP_DEFINE_CONST_OBJ_TYPE(
    esp_panel_mp_board_type,
    MP_QSTR_Board,
    MP_TYPE_FLAG_NONE,
    make_new, (const void *)esp_panel::board::make_new,
    print, (const void *)esp_panel::board::print,
    locals_dict, &esp_panel::board::board_locals_dict
);
