/* file originally from
 * https://gist.github.com/josephg/5e134adf70760ee7e49d
 *
 * modified by github.com/Vik2015
 */
/* this program strips junk characters from each line in the input */
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(void) {
    while(!ferror(stdin) && !feof(stdin)) {
        ssize_t len = 0;
        size_t i = 0;
        char *line = NULL;

        /* read one line from input */
        if ((len = getline(&line, &i, stdin)) < 0) {
            break;
        }

        if (len <= 5)
            /* skip lines 5 chars or less */
            continue;

        for (i = 0; i < 5; i++) {
            if (line[i] == '<') {
                /* now we need to find an end symbol, >, to make sure this is indeed a tag */
                ssize_t j;
                for (j=i + 1; j < len; j++) {
                    if (line[j] == '<' || line[j] == '>')
                        break;
                }
                if (line[j] == '>')
                    /* end tag found, this is (hopefully) a correct XML entry */
                    break;
            }
        }
        fwrite(line + i, 1, len - i, stdout);
    }

    return 0;
}
