#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <stdlib.h>   // For atoi()
#include "date.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

static uint8_t buffer[64];  // Buffer to hold encoded protobuf

// Command to encode date into protobuf
static int cmd_encode_date(const struct shell *shell, size_t argc, char **argv) {
    if (argc != 4) {
        shell_print(shell, "Usage: encode_date <day> <month> <year>");
        return -1;
    }

    Date date = Date_init_zero;
    date.day = atoi(argv[1]);
    date.month = atoi(argv[2]);
    date.year = atoi(argv[3]);

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    if (!pb_encode(&stream, Date_fields, &date)) {
        shell_print(shell, "Encode failed: %s", PB_GET_ERROR(&stream));
        return -1;
    }

    shell_print(shell, "Encoded %d bytes", stream.bytes_written);
    return 0;
}

// Command to decode protobuf back to date
static int cmd_decode_date(const struct shell *shell, size_t argc, char **argv) {
    Date date = Date_init_zero;
    pb_istream_t stream = pb_istream_from_buffer(buffer, sizeof(buffer));

    if (!pb_decode(&stream, Date_fields, &date)) {
        shell_print(shell, "Decode failed: %s", PB_GET_ERROR(&stream));
        return -1;
    }

    shell_print(shell, "Decoded Date: %02d/%02d/%04d", date.day, date.month, date.year);
    return 0;
}

// Register shell commands
SHELL_CMD_ARG_REGISTER(encode_date, NULL, "Encode date <day> <month> <year>", cmd_encode_date, 4, 0);
SHELL_CMD_REGISTER(decode_date, NULL, "Decode last encoded date", cmd_decode_date);
