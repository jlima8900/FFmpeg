/*
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with FFmpeg; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef AVFILTER_QP_TABLE_H
#define AVFILTER_QP_TABLE_H

#include <stdint.h>

#include "libavutil/frame.h"
#include "libavutil/video_enc_params.h"

/**
 * Extract a libpostproc-compatible QP table - an 8-bit QP value per 16x16
 * macroblock, stored in raster order - from AVVideoEncParams side data.
 */
int ff_qp_table_extract(AVFrame *frame, int8_t **table, int *table_w, int *table_h,
                        enum AVVideoEncParamsType *qscale_type);

/**
 * Normalize the qscale factor to a common 0-31 range for use by
 * post-processing filters.
 */
static inline int ff_norm_qscale(int qscale, enum AVVideoEncParamsType type)
{
    switch (type) {
    case AV_VIDEO_ENC_PARAMS_MPEG2:
        /* MPEG-2 stores qscale*2 (0-62), divide by 2 to get 0-31 */
        return qscale >> 1;
    case AV_VIDEO_ENC_PARAMS_H264:
        /* H.264 QP is 0-51, scale to 0-31 range */
        return (qscale * 31 + 25) / 51;
    case AV_VIDEO_ENC_PARAMS_VP9:
        /* VP9 QP is 0-255, scale to 0-31 range */
        return (qscale * 31 + 127) / 255;
    }
    return qscale;
}

#endif // AVFILTER_QP_TABLE_H
