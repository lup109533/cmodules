void code_point_to_utf8(char* str, uchar code_point) {
    char   val[4] = {0, 0, 0, 0};
    size_t cln = 0; // char length

    // 1 byte
    if (code_point < 0x0080) {
        val[0] = 0 | code_point;
        cln    = 1;
    }
    // 2 bytes
    else if (code_point < 0x0800) {
        u8 byte0 = (code_point & 0xF80) >> 6;
        u8 byte1 = (code_point & 0x07F);
        val[0] = 0xC0 | byte0;
        val[1] = 0x80 | byte1;
        cln    = 2;
    }
    // 3 bytes
    else if (code_point < 0x10000) {
        u8 byte0 = (code_point & 0xF000) >> 12;
        u8 byte1 = (code_point & 0x0FC0) >> 6;
        u8 byte2 = (code_point & 0x003F);
        val[0] = 0xE0 | byte0;
        val[1] = 0x80 | byte1;
        val[2] = 0x80 | byte2;
        cln    = 3;
    }
    // 4 bytes
    else {
        u8 byte0 = (code_point & 0x1C0000) >> 18;
        u8 byte1 = (code_point & 0x03F000) >> 12;
        u8 byte2 = (code_point & 0x000FC0) >> 6;
        u8 byte3 = (code_point & 0x00003F);
        val[0] = 0xE0 | byte0;
        val[1] = 0x80 | byte1;
        val[2] = 0x80 | byte2;
        val[3] = 0x80 | byte3;
        cln    = 4;
    }

    strncpy(str, val, cln);
}

uchar utf8_to_code_point(char* str) {
    if (str == NULL)
        return 0;

    uchar utf8_char;
    char  first = str[0]; // First byte determines size

    // 0xxxxxxx --> 1 byte
    if ((first & 0x80) == 0) {
        return (uchar) first;
    }
    // 110xxxxx --> 2 bytes
    else if ((first & 0xD0) == 0xC0) {
        utf8_char = (uchar) ((u16*) str)[0];
        uchar byte0 = utf8_char & 0x3F;
        uchar byte1 = utf8_char & (0x1F << 8);
        return (byte1 >> 2) | byte0;
    }
    // 1110xxxx --> 3 bytes
    else if ((first & 0xF0) == 0xD0) {
        utf8_char = (uchar) (((u32*) str)[0] >> 8);
        uchar byte0 = utf8_char & 0x3F;
        uchar byte1 = utf8_char & (0x3F << 8);
        uchar byte2 = utf8_char & (0x0F << 16);
        return (byte2 >> 4) | (byte1 >> 2) | byte0;
    }
    // 11110xxx --> 4 bytes
    else if ((first & 0xF8) == 0xF0) {
        utf8_char = (uchar) (((u32*) str)[0]);
        uchar byte0 = utf8_char & 0x3F;
        uchar byte1 = utf8_char & (0x3F << 8);
        uchar byte2 = utf8_char & (0x3F << 16);
        uchar byte3 = utf8_char & (0x07 << 24);
        return (byte3 >> 6) | (byte2 >> 4) | (byte1 >> 2) | byte0;
    }
    // Something else
    else {
        return 0;
    }
}

size_t code_point_size(uchar code_point) {
    // 1 byte
    if (code_point < 0x0080) {
        return 1;
    }
    // 2 bytes
    else if (code_point < 0x0800) {
        return 2;
    }
    // 3 bytes
    else if (code_point < 0x10000) {
        return 3;
    }
    // 4 bytes
    else {
        return 4;
    }
}
