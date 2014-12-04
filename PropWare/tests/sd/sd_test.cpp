/**
 * @file    sd_test.cpp
 *
 * @author  David Zemon
 *
 * Hardware:
 *      SD card connected with the following pins:
 *          - MOSI = P0
 *          - MISO = P1
 *          - SCLK = P2
 *          - CS   = P4
 *
 * @copyright
 * The MIT License (MIT)<br>
 * <br>Copyright (c) 2013 David Zemon<br>
 * <br>Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:<br>
 * <br>The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.<br>
 * <br>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <PropWare/sd.h>
#include "../PropWareTests.h"

PropWare::SD *testable;

const PropWare::Pin::Mask MOSI = PropWare::Pin::P0;
const PropWare::Pin::Mask MISO = PropWare::Pin::P1;
const PropWare::Pin::Mask SCLK = PropWare::Pin::P2;
const PropWare::Pin::Mask CS   = PropWare::Pin::P4;

TEARDOWN {
}

TEST(Start) {
    MSG_IF_FAIL(1, ASSERT_FALSE(testable->start()),
                "Failed to start %s", ":(");

    tearDown();
}

TEST(ReadBlock) {
    ASSERT_FALSE(testable->start());

    // Create a buffer and initialize all values to 0. Surely the first sector
    // of the SD card won't be _all_ zeros!
    uint8_t buffer[PropWare::SD::SECTOR_SIZE];
    for (int i = 0; i < sizeof(buffer); ++i)
        buffer[i] = 0;

    // Read in a block...
    ASSERT_FALSE(testable->read_data_block(0, buffer));

    // And make sure at least _one_ of the bytes is non-zero
    for (int j = 0; j < sizeof(buffer); ++j)
        if (buffer[j])
            tearDown();

    // If the whole loop finished, that means none of the bytes changed. That
    // _can't_ be right so go ahead and call it a failure
    FAIL();
}

int main () {
    START(SDTest);

    testable = new PropWare::SD(PropWare::SPI::get_instance(), MOSI, MISO,
                                SCLK, CS);

    RUN_TEST(Start);
    RUN_TEST(ReadBlock);

    COMPLETE();
}