#ifndef GENERAL_SHADER_FRAG_H
#define GENERAL_SHADER_FRAG_H

enum class DisplayMode {
	SL, SR,
	P, X, PA, XA,
	TB, BT, TBA, BTA,
	IR1, IR2, IC1, IC2, CK1, CK2,
	ML, MR, MB,
	A_RC_G, A_RC_HC, A_RC_FC, A_RC_D,
	A_YB_G, A_YB_HC, A_YB_FC, A_YB_D,
	A_GM_G, A_GM_HC, A_GM_FC, A_GM_D,
};

static const constexpr auto general_shader_frag = R"(
varying lowp vec2 v_coord;
uniform sampler2D sampler;
uniform int mode;

void mode_0() { // Single view left
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    gl_FragColor = texture2D(sampler, uv);
}

void mode_1() { // Single view right
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0 + 0.5;
    gl_FragColor = texture2D(sampler, uv);
}

void mode_2() { // Parallel view
    lowp vec2 uv = v_coord;
    uv.y = uv.y * 2.0 - 0.5;
    if (uv.y >= 0.0 && uv.y < 1.0)
        gl_FragColor = texture2D(sampler, uv);
}

void mode_3() { // Cross view
    lowp vec2 uv = v_coord;
    uv.x = uv.x - 0.5;
    uv.y = uv.y * 2.0 - 0.5;
    if (uv.y >= 0.0 && uv.y < 1.0)
        gl_FragColor = texture2D(sampler, uv);
}

void mode_4() { // Parallel anamorphic view
    lowp vec2 uv = v_coord;
    gl_FragColor = texture2D(sampler, uv);
}

void mode_5() { // Cross anamorphic view
    lowp vec2 uv = v_coord;
    uv.x = uv.x - 0.5;
    gl_FragColor = texture2D(sampler, uv);
}

void mode_6() { // Top-Bottom view
    lowp vec2 uv = v_coord;
    if (uv.x >= 0.25 && uv.x < 0.75) {
        uv.x = uv.x - 0.25 + ((uv.y < 0.5) ? 0.5 : 0.0);
        uv.y = uv.y * 2.0;
        gl_FragColor = texture2D(sampler, uv);
    }
}

void mode_7() { // Bottom-top view
    lowp vec2 uv = v_coord;
    if (uv.x >= 0.25 && uv.x < 0.75) {
        uv.x = uv.x - 0.25 + ((uv.y >= 0.5) ? 0.5 : 0.0);
        uv.y = uv.y * 2.0;
        gl_FragColor = texture2D(sampler, uv);
    }
}

void mode_8() { // Top-Bottom anamorphic view
    lowp vec2 uv = v_coord;
    uv.x = (uv.y >= 0.5) ? uv.x / 2.0 : uv.x / 2.0 + 0.5;
    uv.y = uv.y * 2.0;
    gl_FragColor = texture2D(sampler, uv);
}

void mode_9() { // Bottom-top anamorphic view
    lowp vec2 uv = v_coord;
    uv.x = (uv.y < 0.5) ? uv.x / 2.0 : uv.x / 2.0 + 0.5;
    uv.y = uv.y * 2.0;
    gl_FragColor = texture2D(sampler, uv);
}

void mode_10() { // Interleaved lines 1
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    float coord = gl_FragCoord.y;
    gl_FragColor = ((mod(coord, 2.0)) >= 1.0) ?
        texture2D(sampler, uv) : texture2D(sampler, uv + vec2(0.5, 0.0));
}

void mode_11() { // Interleaved lines 2
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    float coord = gl_FragCoord.y + 1.0;
    gl_FragColor = ((mod(coord, 2.0)) >= 1.0) ?
        texture2D(sampler, uv) : texture2D(sampler, uv + vec2(0.5, 0.0));
}

void mode_12() { // Interleaved columns 1
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    float coord = gl_FragCoord.x;
    gl_FragColor = ((mod(coord, 2.0)) >= 1.0) ?
        texture2D(sampler, uv) : texture2D(sampler, uv + vec2(0.5, 0.0));
}

void mode_13() { // Interleaved columns 2
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    float coord = gl_FragCoord.x + 1.0;
    gl_FragColor = ((mod(coord, 2.0)) >= 1.0) ?
        texture2D(sampler, uv) : texture2D(sampler, uv + vec2(0.5, 0.0));
}

void mode_14() { // Checkerboard 1
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    float coord = mod(gl_FragCoord.x, 2.0) + mod(gl_FragCoord.y, 2.0);
    gl_FragColor = ((mod(coord, 2.0)) >= 1.0) ?
        texture2D(sampler, uv) : texture2D(sampler, uv + vec2(0.5, 0.0));
}

void mode_15() { // Checkerboard 2
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    float coord = mod(gl_FragCoord.x, 2.0) + mod(gl_FragCoord.y, 2.0) + 1.0;
    gl_FragColor = ((mod(coord, 2.0)) >= 1.0) ?
        texture2D(sampler, uv) : texture2D(sampler, uv + vec2(0.5, 0.0));
}

void mode_16() { // Mirrored left
    lowp vec2 uv = v_coord;
    if (uv.x < 0.5) uv.x = 0.5 - uv.x;
    uv.y = uv.y * 2.0 - 0.5;
    if (uv.y >= 0.0 && uv.y < 1.0)
        gl_FragColor = texture2D(sampler, uv);
}

void mode_17() { // Mirrored right
    lowp vec2 uv = v_coord;
    if (uv.x >= 0.5) uv.x = 1.5 - uv.x;
    uv.y = 2.0 * uv.y - 0.5;
    if (uv.y >= 0.0 && uv.y < 1.0)
        gl_FragColor = texture2D(sampler, uv);
}

void mode_18() { // Mirrored both
    lowp vec2 uv = v_coord;
    uv.x = (uv.x < 0.5) ? 0.5 - uv.x : 1.5 - uv.x;
    uv.y = uv.y * 2.0 - 0.5;
    if (uv.y >= 0.0 && uv.y < 1.0)
        gl_FragColor = texture2D(sampler, uv);
}

void mode_19() { // Anaglyph Red/Cyan Gray
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    lowp vec4 l = texture2D(sampler, uv);
    lowp vec4 r = texture2D(sampler, uv + vec2(0.5, 0.0));
    lowp mat4 mat_l = mat4(
        +0.299, +0.000, +0.000, 0.0,
        +0.587, +0.000, +0.000, 0.0,
        +0.114, +0.000, +0.000, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    lowp mat4 mat_r = mat4(
        +0.000, +0.299, +0.299, 0.0,
        +0.000, +0.587, +0.587, 0.0,
        +0.000, +0.114, +0.114, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    gl_FragColor = mat_l * l + mat_r * r;
}

void mode_20() { // Anaglyph Red/Cyan Half Colors
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    lowp vec4 l = texture2D(sampler, uv);
    lowp vec4 r = texture2D(sampler, uv + vec2(0.5, 0.0));
    lowp mat4 mat_l = mat4(
        +0.299, +0.000, +0.000, 0.0,
        +0.587, +0.000, +0.000, 0.0,
        +0.114, +0.000, +0.000, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    lowp mat4 mat_r = mat4(
        +0.000, +0.000, +0.000, 0.0,
        +0.000, +1.000, +0.000, 0.0,
        +0.000, +0.000, +1.000, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    gl_FragColor = mat_l * l + mat_r * r;
}

void mode_21() { // Anaglyph Red/Cyan Full Colors
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    lowp vec4 l = texture2D(sampler, uv);
    lowp vec4 r = texture2D(sampler, uv + vec2(0.5, 0.0));
    lowp mat4 mat_l = mat4(
        +1.000, +0.000, +0.000, 0.0,
        +0.000, +0.000, +0.000, 0.0,
        +0.000, +0.000, +0.000, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    lowp mat4 mat_r = mat4(
        +0.000, +0.000, +0.000, 0.0,
        +0.000, +1.000, +0.000, 0.0,
        +0.000, +0.000, +1.000, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    gl_FragColor = mat_l * l + mat_r * r;
}

void mode_22() { // Anaglyph Red/Cyan Dubois
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    lowp vec4 l = texture2D(sampler, uv);
    lowp vec4 r = texture2D(sampler, uv + vec2(0.5, 0.0));
    lowp mat4 mat_l = mat4(
        +0.456, -0.040, -0.015, 0.0,
        +0.500, -0.038, -0.021, 0.0,
        +0.176, -0.016, -0.005, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    lowp mat4 mat_r = mat4(
        -0.043, +0.378, -0.072, 0.0,
        -0.088, +0.734, -0.113, 0.0,
        -0.002, -0.018, +1.226, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    gl_FragColor = mat_l * l + mat_r * r;
}

void mode_23() { // Anaglyph Yellow/Blue Gray
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    lowp vec4 l = texture2D(sampler, uv);
    lowp vec4 r = texture2D(sampler, uv + vec2(0.5, 0.0));
    lowp mat4 mat_l = mat4(
        +0.299, +0.299, +0.000, 0.0,
        +0.587, +0.587, +0.000, 0.0,
        +0.114, +0.114, +0.000, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    lowp mat4 mat_r = mat4(
        +0.000, +0.000, +0.299, 0.0,
        +0.000, +0.000, +0.587, 0.0,
        +0.000, +0.000, +0.114, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    gl_FragColor = mat_l * l + mat_r * r;
}

void mode_24() { // Anaglyph Yellow/Blue Half Colors
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    lowp vec4 l = texture2D(sampler, uv);
    lowp vec4 r = texture2D(sampler, uv + vec2(0.5, 0.0));
    lowp mat4 mat_l = mat4(
        +1.000, +0.000, +0.000, 0.0,
        +0.000, +1.000, +0.000, 0.0,
        +0.000, +0.000, +0.000, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    lowp mat4 mat_r = mat4(
        +0.000, +0.000, +0.299, 0.0,
        +0.000, +0.000, +0.587, 0.0,
        +0.000, +0.000, +0.114, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    gl_FragColor = mat_l * l + mat_r * r;
}

void mode_25() { // Anaglyph Yellow/Blue Full Colors
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    lowp vec4 l = texture2D(sampler, uv);
    lowp vec4 r = texture2D(sampler, uv + vec2(0.5, 0.0));
    lowp mat4 mat_l = mat4(
        +1.000, +0.000, +0.000, 0.0,
        +0.000, +1.000, +0.000, 0.0,
        +0.000, +0.000, +0.000, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    lowp mat4 mat_r = mat4(
        +0.000, +0.000, +0.000, 0.0,
        +0.000, +0.000, +0.000, 0.0,
        +0.000, +0.000, +1.000, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    gl_FragColor = mat_l * l + mat_r * r;
}

void mode_26() { // Anaglyph Yellow/Blue Dubois
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    lowp vec4 l = texture2D(sampler, uv);
    lowp vec4 r = texture2D(sampler, uv + vec2(0.5, 0.0));
    lowp mat4 mat_l = mat4(
        +1.062, -0.026, -0.038, 0.0,
        -0.205, +0.908, -0.173, 0.0,
        +0.299, +0.068, +0.022, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    lowp mat4 mat_r = mat4(
        -0.016, +0.006, +0.094, 0.0,
        -0.123, +0.062, +0.185, 0.0,
        -0.017, -0.017, +0.911, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    gl_FragColor = mat_l * l + mat_r * r;
}

void mode_27() { // Anaglyph Green/Magenta Gray
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    lowp vec4 l = texture2D(sampler, uv);
    lowp vec4 r = texture2D(sampler, uv + vec2(0.5, 0.0));
    lowp mat4 mat_l = mat4(
        +0.000, +0.299, +0.000, 0.0,
        +0.000, +0.587, +0.000, 0.0,
        +0.000, +0.114, +0.000, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    lowp mat4 mat_r = mat4(
        +0.299, +0.000, +0.299, 0.0,
        +0.587, +0.000, +0.587, 0.0,
        +0.114, +0.000, +0.114, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    gl_FragColor = mat_l * l + mat_r * r;
}

void mode_28() { // Anaglyph Green/Magenta Half Colors
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    lowp vec4 l = texture2D(sampler, uv);
    lowp vec4 r = texture2D(sampler, uv + vec2(0.5, 0.0));
    lowp mat4 mat_l = mat4(
        +0.000, +0.299, +0.000, 0.0,
        +0.000, +0.587, +0.000, 0.0,
        +0.000, +0.114, +0.000, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    lowp mat4 mat_r = mat4(
        +1.000, +0.000, +0.000, 0.0,
        +0.000, +0.000, +0.000, 0.0,
        +0.000, +0.000, +1.000, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    gl_FragColor = mat_l * l + mat_r * r;
}

void mode_29() { // Anaglyph Green/Magenta Full Colors
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    lowp vec4 l = texture2D(sampler, uv);
    lowp vec4 r = texture2D(sampler, uv + vec2(0.5, 0.0));
    lowp mat4 mat_l = mat4(
        +0.000, +0.000, +0.000, 0.0,
        +0.000, +1.000, +0.000, 0.0,
        +0.000, +0.000, +0.000, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    lowp mat4 mat_r = mat4(
        +1.000, +0.000, +0.000, 0.0,
        +0.000, +0.000, +0.000, 0.0,
        +0.000, +0.000, +1.000, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    gl_FragColor = mat_l * l + mat_r * r;
}

void mode_30() { // Anaglyph Green/Magenta Dubois
    lowp vec2 uv = v_coord;
    uv.x = uv.x / 2.0;
    lowp vec4 l = texture2D(sampler, uv);
    lowp vec4 r = texture2D(sampler, uv + vec2(0.5, 0.0));
    lowp mat4 mat_l = mat4(
        -0.062, +0.284, -0.015, 0.0,
        -0.158, +0.668, -0.027, 0.0,
        -0.039, +0.143, +0.021, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    lowp mat4 mat_r = mat4(
        +0.529, -0.016, +0.009, 0.0,
        +0.705, -0.015, +0.075, 0.0,
        +0.024, -0.065, +0.937, 0.0,
        +0.000, +0.000, +0.000, 1.0
    );
    gl_FragColor = mat_l * l + mat_r * r;
}

void main() {
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);

    // Dichotomic switch
    if (mode < 16) {
        if (mode < 8) {
            if (mode < 4) {
                if (mode < 2) {
                    if (mode < 1) {
                        mode_0();
                    } else {
                        mode_1();
                    }
                } else {
                    if (mode < 3) {
                        mode_2();
                    } else {
                        mode_3();
                    }
                }
            } else {
                if (mode < 6) {
                    if (mode  < 5) {
                        mode_4();
                    } else {
                        mode_5();
                    }
                } else {
                    if (mode < 7) {
                        mode_6();
                    } else {
                        mode_7();
                    }
                }
            }
        } else {
            if (mode < 12) {
                if (mode  < 10) {
                    if (mode < 9) {
                        mode_8();
                    } else {
                        mode_9();
                    }
                } else {
                    if (mode < 11) {
                        mode_10();
                    } else {
                        mode_11();
                    }
                }
            } else {
                if (mode < 14) {
                    if (mode < 13) {
                        mode_12();
                    } else {
                        mode_13();
                    }
                } else {
                    if (mode < 15) {
                        mode_14();
                    } else {
                        mode_15();
                    }
                }
            }
        }
    } else {
        if (mode < 24) {
            if (mode < 20) {
                if (mode < 18) {
                    if (mode < 17) {
                        mode_16();
                    } else {
                        mode_17();
                    }
                } else {
                    if (mode < 19) {
                        mode_18();
                    } else {
                        mode_19();
                    }
                }
            } else {
                if (mode < 22) {
                    if (mode  < 21) {
                        mode_20();
                    } else {
                        mode_21();
                    }
                } else {
                    if (mode < 23) {
                        mode_22();
                    } else {
                        mode_23();
                    }
                }
            }
        } else {
            if (mode < 28) {
                if (mode  < 26) {
                    if (mode < 25) {
                        mode_24();
                    } else {
                        mode_25();
                    }
                } else {
                    if (mode < 27) {
                        mode_26();
                    } else {
                        mode_27();
                    }
                }
            } else {
                if (mode < 30) {
                    if (mode < 29) {
                        mode_28();
                    } else {
                        mode_29();
                    }
                } else {
                    if (mode < 31) {
                        mode_30();
                    } else {
                        // No mode_31
                    }
                }
            }
        }
    }
}
)";

#endif // GENERAL_SHADER_FRAG_H
