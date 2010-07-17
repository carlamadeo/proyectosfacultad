
// used for layouts of graphics on a circle

float
lookup(int angle)
{
 float ret_val = 0.0;

 if (angle == 0) ret_val = 1.000000;
 if (angle == 1) ret_val = 0.999848;
 if (angle == 2) ret_val = 0.999391;
 if (angle == 3) ret_val = 0.998630;
 if (angle == 4) ret_val = 0.997564;
 if (angle == 5) ret_val = 0.996195;
 if (angle == 6) ret_val = 0.994522;
 if (angle == 7) ret_val = 0.992546;
 if (angle == 8) ret_val = 0.990268;
 if (angle == 9) ret_val = 0.987688;
 if (angle == 10) ret_val = 0.984808;
 if (angle == 11) ret_val = 0.981627;
 if (angle == 12) ret_val = 0.978148;
 if (angle == 13) ret_val = 0.974370;
 if (angle == 14) ret_val = 0.970296;
 if (angle == 15) ret_val = 0.965926;
 if (angle == 16) ret_val = 0.961262;
 if (angle == 17) ret_val = 0.956305;
 if (angle == 18) ret_val = 0.951057;
 if (angle == 19) ret_val = 0.945519;
 if (angle == 20) ret_val = 0.939693;
 if (angle == 21) ret_val = 0.933580;
 if (angle == 22) ret_val = 0.927184;
 if (angle == 23) ret_val = 0.920505;
 if (angle == 24) ret_val = 0.913545;
 if (angle == 25) ret_val = 0.906308;
 if (angle == 26) ret_val = 0.898794;
 if (angle == 27) ret_val = 0.891007;
 if (angle == 28) ret_val = 0.882948;
 if (angle == 29) ret_val = 0.874620;
 if (angle == 30) ret_val = 0.866025;
 if (angle == 31) ret_val = 0.857167;
 if (angle == 32) ret_val = 0.848048;
 if (angle == 33) ret_val = 0.838671;
 if (angle == 34) ret_val = 0.829038;
 if (angle == 35) ret_val = 0.819152;
 if (angle == 36) ret_val = 0.809017;
 if (angle == 37) ret_val = 0.798635;
 if (angle == 38) ret_val = 0.788011;
 if (angle == 39) ret_val = 0.777146;
 if (angle == 40) ret_val = 0.766044;
 if (angle == 41) ret_val = 0.754710;
 if (angle == 42) ret_val = 0.743145;
 if (angle == 43) ret_val = 0.731354;
 if (angle == 44) ret_val = 0.719340;
 if (angle == 45) ret_val = 0.707107;
 if (angle == 46) ret_val = 0.694658;
 if (angle == 47) ret_val = 0.681998;
 if (angle == 48) ret_val = 0.669131;
 if (angle == 49) ret_val = 0.656059;
 if (angle == 50) ret_val = 0.642788;
 if (angle == 51) ret_val = 0.629320;
 if (angle == 52) ret_val = 0.615662;
 if (angle == 53) ret_val = 0.601815;
 if (angle == 54) ret_val = 0.587785;
 if (angle == 55) ret_val = 0.573576;
 if (angle == 56) ret_val = 0.559193;
 if (angle == 57) ret_val = 0.544639;
 if (angle == 58) ret_val = 0.529919;
 if (angle == 59) ret_val = 0.515038;
 if (angle == 60) ret_val = 0.500000;
 if (angle == 61) ret_val = 0.484810;
 if (angle == 62) ret_val = 0.469472;
 if (angle == 63) ret_val = 0.453990;
 if (angle == 64) ret_val = 0.438371;
 if (angle == 65) ret_val = 0.422618;
 if (angle == 66) ret_val = 0.406737;
 if (angle == 67) ret_val = 0.390731;
 if (angle == 68) ret_val = 0.374607;
 if (angle == 69) ret_val = 0.358368;
 if (angle == 70) ret_val = 0.342020;
 if (angle == 71) ret_val = 0.325568;
 if (angle == 72) ret_val = 0.309017;
 if (angle == 73) ret_val = 0.292372;
 if (angle == 74) ret_val = 0.275637;
 if (angle == 75) ret_val = 0.258819;
 if (angle == 76) ret_val = 0.241922;
 if (angle == 77) ret_val = 0.224951;
 if (angle == 78) ret_val = 0.207912;
 if (angle == 79) ret_val = 0.190809;
 if (angle == 80) ret_val = 0.173648;
 if (angle == 81) ret_val = 0.156434;
 if (angle == 82) ret_val = 0.139173;
 if (angle == 83) ret_val = 0.121869;
 if (angle == 84) ret_val = 0.104528;
 if (angle == 85) ret_val = 0.087156;
 if (angle == 86) ret_val = 0.069756;
 if (angle == 87) ret_val = 0.052336;
 if (angle == 88) ret_val = 0.034899;
 if (angle == 89) ret_val = 0.017452;
 return ret_val;
}

// the angle must be between 0-359
int
getX(int angle, int radius)
{
    int multi = 1;
    if (angle < 270 && angle > 90) multi = -1;
    if (angle >= 180) angle = angle - 180;
    if (angle < 180 && angle > 90) angle = 180 - angle;

    return to_int(lookup(angle) * multi * radius);
}

// the angle must be between 0-359
int
getY(int angle, int radius)
{
    int multi = 1;
    if (angle > 180) multi = -1;
    if (angle >= 180) angle = angle - 180;
    if (angle < 180 && angle > 90) angle = 180 - angle;

    return to_int(lookup(90 - angle) * multi * radius);
}
