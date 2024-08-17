// imrcsz_control_base_ms. RCSZ.
// version: [x] NORMAL [ ] SIMD [ ] INTEL_SIMD [ ] AMD_SIMD
// v20240622.

#ifndef __IMRCSZ_CONTROL_BASE_MS_H
#define __IMRCSZ_CONTROL_BASE_MS_H

#include "imgui.h"
#include "imgui_internal.h"

#define IMGUI_ITEM_SPAC ImGui::GetStyle().ItemSpacing.x

#define IM_MATHS_PI 3.14159265f
#define IM_MATHS_CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))
#define IM_MATHS_DEGTORAD(deg) ((deg) * IM_MATHS_PI / 180.0f)
#define IM_MATHS_DISTANCE(pos1, pos2) std::sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y))

#define IM_CVT_FP32COL32(R, G, B, A) IM_COL32(ImU32(R * 255.0f), ImU32(G * 255.0f), ImU32(B * 255.0f), ImU32(A * 255.0f))
#define IM_CVT_FMTFP32(V)		     ImU32((V) * 255.0f)

#define IMVEC4_TO_COLU32(COL) IM_COL32(IM_CVT_FMTFP32((COL).x), IM_CVT_FMTFP32((COL).y), IM_CVT_FMTFP32((COL).z), IM_CVT_FMTFP32((COL).w))

#ifdef IMRCSZ_CONTROL_BASE_MATHS
static inline ImVec2 operator+(const ImVec2& v, const ImVec2& s) { return ImVec2(v.x + s.x, v.y + s.y); }
static inline ImVec2 operator+(const ImVec2& v, float s)         { return ImVec2(v.x + s, v.y + s); }

static inline ImVec2 operator-(const ImVec2& v, const ImVec2& s) { return ImVec2(v.x - s.x, v.y - s.y); }
static inline ImVec2 operator-(const ImVec2& v, float s)         { return ImVec2(v.x - s, v.y - s); }
static inline ImVec2 operator-(float s, const ImVec2& v)         { return ImVec2(s - v.x, s - v.y); }

static inline ImVec2 operator*(const ImVec2& v, const ImVec2& s) { return ImVec2(v.x * s.x, v.y * s.y); }
static inline ImVec2 operator*(const ImVec2& v, float s)         { return ImVec2(v.x * s, v.y * s); }

static inline ImVec2 operator/(const ImVec2& v, const ImVec2& s) { return ImVec2(v.x / s.x, v.y / s.y); }
static inline ImVec2 operator/(const ImVec2& v, float s)         { return ImVec2(v.x / s, v.y / s); }
static inline ImVec2 operator/(float s, const ImVec2& v)         { return ImVec2(s / v.x, s / v.y); }

static inline ImVec2& operator+=(ImVec2& v1, const ImVec2& v2) { return v1 = v1 + v2; }
static inline ImVec2& operator-=(ImVec2& v1, const ImVec2& v2) { return v1 = v1 - v2; }
static inline ImVec2& operator*=(ImVec2& v1, const ImVec2& v2) { return v1 = v1 * v2; }
static inline ImVec2& operator/=(ImVec2& v1, const ImVec2& v2) { return v1 = v1 / v2; }

static inline ImVec4 operator+(const ImVec4& v, const ImVec4& s) { return ImVec4(v.x + s.x, v.y + s.y, v.z + s.z, v.w + s.w); }
static inline ImVec4 operator+(const ImVec4& v, float s)         { return ImVec4(v.x + s, v.y + s, v.z + s, v.w + s); }

static inline ImVec4 operator-(const ImVec4& v, const ImVec4& s) { return ImVec4(v.x - s.x, v.y - s.y, v.z - s.z, v.w - s.w); }
static inline ImVec4 operator-(const ImVec4& v, float s)         { return ImVec4(v.x - s, v.y - s, v.z - s, v.w - s); }
static inline ImVec4 operator-(float s, const ImVec4& v)         { return ImVec4(s - v.x, s - v.y, s - v.z, s - v.w); }

static inline ImVec4 operator*(const ImVec4& v, const ImVec4& s) { return ImVec4(v.x * s.x, v.y * s.y, v.z * s.z, v.w * s.w); }
static inline ImVec4 operator*(const ImVec4& v, float s)         { return ImVec4(v.x * s, v.y * s, v.z * s, v.w * s); }

static inline ImVec4 operator/(const ImVec4& v, const ImVec4& s) { return ImVec4(v.x / s.x, v.y / s.y, v.z / s.z, v.w / s.w); }
static inline ImVec4 operator/(const ImVec4& v, float s)         { return ImVec4(v.x / s, v.y / s, v.z / s, v.w / s); }
static inline ImVec4 operator/(float s, const ImVec4& v)         { return ImVec4(s / v.x, s / v.y, s / v.z, s / v.w); }

static inline ImVec4& operator+=(ImVec4& v1, const ImVec4& v2) { return v1 = v1 + v2; }
static inline ImVec4& operator-=(ImVec4& v1, const ImVec4& v2) { return v1 = v1 - v2; }
static inline ImVec4& operator*=(ImVec4& v1, const ImVec4& v2) { return v1 = v1 * v2; }
static inline ImVec4& operator/=(ImVec4& v1, const ImVec4& v2) { return v1 = v1 / v2; }

#endif
#endif