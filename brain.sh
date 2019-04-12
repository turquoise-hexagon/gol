#!/usr/bin/env bash
#
# brian's brain

X=30
Y=30

declare -A uni cpy

for ((i = 0; i < X; i++)) {
    for ((j = 0; j < Y; j++)) {
        ((uni[$i $j] = RANDOM % 3))
    }
}

printf '\e[2J\e[?25l'
trap $'printf \e[?25h' EXIT

for ((;;)) {
    out='\e[H'

    for ((i = 0; i < X; i++)) {
        for ((j = 0; j < Y; j++)) {
            cpt=0

            for a in -1 0 1; {
                for b in -1 0 1; {
                    ((a || b)) && {
                        ((
                            x = i + a, x = x < 0 ? X - 1 : x % X,
                            y = j + b, y = y < 0 ? Y - 1 : y % Y
                        ))

                        ((uni[$x $y] == 2 && cpt++))
                    }
                }
            }

            case ${uni[$i $j]} in
                1) out+='\e[31m# \e[m'; cpy[$i $j]=0;;
                2) out+='\e[32m# \e[m'; cpy[$i $j]=1;;
                0) out+='\e[30m# \e[m'; ((cpt == 2)) && cpy[$i $j]=2
            esac
        }
        
        out+='\n'
    }

    printf '%b' "$out"

    for i in "${!cpy[@]}"; {
        uni[$i]=${cpy[$i]}
    }
}
