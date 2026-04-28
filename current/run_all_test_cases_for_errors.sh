#!/bin/bash
# Progress Bar code modified from: https://www.baeldung.com/linux/command-line-progress-bar
# Parallel code written by Grok (xAI)
# 12/2025 with Grok v4.

# Needs sudo apt install parallel

bar_size=40
bar_char_done="#"
bar_char_todo="-"
bar_percentage_scale=2

function show_progress {
    current="$1"
    total="$2"

    # calculate the progress in percentage 
    percent=$(bc <<< "scale=$bar_percentage_scale; 100 * $current / $total" )
    # The number of done and todo characters
    done=$(bc <<< "scale=0; $bar_size * $percent / 100" )
    todo=$(bc <<< "scale=0; $bar_size - $done" )

    # build the done and todo sub-bars
    done_sub_bar=$(printf "%${done}s" | tr " " "${bar_char_done}")
    todo_sub_bar=$(printf "%${todo}s" | tr " " "${bar_char_todo}")

    # output the bar
    echo -ne "\rProgress : [${done_sub_bar}${todo_sub_bar}] ${percent}%"

    if [ $total -eq $current ]; then
        echo -e "\nDONE"
    fi
}

# BEGIN DMO SCRIPT 

function run_test {
    file="$1"
    type="$2"
    basename=$(basename "${file%.c}")
    tmpdir=$(mktemp -d /tmp/compiler-test.XXXXXX)
    trap "rm -rf '$tmpdir'" EXIT

    cp Makefile frontend_lex.l frontend_parser.y parserstack.c parserstack.h backend_lex.l backend_parser.y frontend_parsetree.c backend_parsetree.c frontend_ir.c symboltable.h backend_x64.c "$tmpdir"
    # If gdb_script.txt exists and is needed, uncomment the following:
    # [ -f gdb_script.txt ] && cp gdb_script.txt "$tmpdir"

    mkdir -p "$tmpdir/test-cases/$type"
    cp "test-cases/$type/driver.cpp" "test-cases/$type/evaluator.cpp" "$tmpdir/test-cases/$type/"
    cp "$file" "$tmpdir/prog.c"

    cd "$tmpdir" || exit 1
    make prog=prog.c type=$type &> "test.log"
    exitcode=$?

    if [ $exitcode -eq 0 ]; then
        make clean &> /dev/null
    else
        cd - || exit 1
        mkdir -p logs
        cp "$tmpdir/test.log" "logs/${basename}.log"
        echo
        echo " **** FAILED: $file **** "
        echo " See logs/${basename}.log "
        echo
        return 1
    fi

    cd - || exit 1
    return 0
}

set -e

mapfile -t return_int_files < <(find "./test-cases/return-int/" -type f -name "test-case-*.c")
mapfile -t pass_int_files < <(find "./test-cases/pass-int/" -type f -name "test-case-*.c")

return_int_count=${#return_int_files[@]}
pass_int_count=${#pass_int_files[@]}
total_test_case_count=$((return_int_count + pass_int_count))

all_tests=()
for f in "${return_int_files[@]}"; do all_tests+=("$f return-int"); done
for f in "${pass_int_files[@]}"; do all_tests+=("$f pass-int"); done

export -f run_test
printf "%s\n" "${all_tests[@]}" | parallel --colsep ' ' --bar --halt now,fail=1 -j8 run_test {1} {2}

echo
echo
echo " Tests performed: $total_test_case_count "
echo " ***** ALL TESTS PASSED!! ***** "
echo
echo