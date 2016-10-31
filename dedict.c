/* file originally from
 * https://gist.github.com/josephg/5e134adf70760ee7e49d
 *
 * modified by github.com/Vik2015
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "zlib.h"

#define CHUNK 16384

/*
 40 Length of the zlib stream
 4c 0020
 54 0275 number of blocks
 
 60 808c pointer to the next block
 64 8088 length of the first block
 68 047a4a length of the unpacked block
 6c start of the zlib stream
 
 80fc second block
 
 13cd134
 13cd174
*/

int unpack(unsigned char *in, int len)
{
    int ret,outed=0;
    unsigned have;
    z_stream strm;
    unsigned char out[CHUNK];
    
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;
    strm.avail_in = len;
    strm.next_in = in;
    do {
        strm.avail_out = CHUNK;
        strm.next_out = out;
        ret = inflate(&strm, Z_NO_FLUSH);
        assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
        switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
        }
        have = CHUNK - strm.avail_out;
        int off = 0;

        if (have - off <= 3) {
            fprintf(stderr, "error: could not find entry\n");
        }

        if (fwrite(out + off, have - off, 1, stdout) != 1 || ferror(stdout)) {
            (void)inflateEnd(&strm);
            fprintf(stderr, "error: zlib error: %s\n", strerror(errno));
            return Z_ERRNO;
        }

        outed+=have;
    } while (strm.avail_out == 0);
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

int main(int argc,char **argv) {
    FILE *fin = NULL;
    int limit = 0,
        blen = 0,
        p = 0,
        l = 0,
        bcnt = 0;
    unsigned char *buf = NULL;
    assert(argc >= 2);

    if ((fin = fopen(argv[1], "rb"))) {
        /* skip over the zlib stream */
        fseek(fin, 0x40, SEEK_SET);
        fread(&l, 1, 4, fin);
        limit = 0x40 + l;
        p = 0x60;
        do {
            /* read and decompress blocks */
            fseek(fin, p, SEEK_SET);
            fread(&l, 1, 4, fin);
            if (blen < l) {
                if(buf != NULL) free(buf);
                blen = l;
                buf = (unsigned char *)malloc(blen);
            }
            
            fread(buf, 1, l, fin);
            if (unpack(buf+8, l-8) != Z_OK) {
                fprintf(stderr, "error: zlib error\n");
                return 1;
            }
            p += 4 + l;
            ++bcnt;
        } while(p < limit);

        free(buf);
        fclose(fin);
    } else {
        fprintf(stderr, "error: failed to open file: %s\n", strerror(errno));
        return 1;
    }

    return 0;
}
