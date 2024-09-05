#!/bin/sh

# Run all of the examples.

set -e

find . -type d -depth 1 \
    | grep -v .git \
    | grep -v scripts \
    | xargs -I {} sh -c "\
        cd {} \
        && make > /dev/null 2>&1 \
        || ( \
            echo {} is sad \
            && exit 1 \
        )
    "
