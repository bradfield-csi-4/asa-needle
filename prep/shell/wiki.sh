#!/usr/local/bin/bash

toc_lines_regex='<li class="toclevel-'

# get_paragraphs='<p[\s\S]+?<\/p>'
get_paragraphs='^<p>([\s\S])+'
# body_without_tables='(<\/td>|<body)[\s\S]+?(<td>|<\/body>)'
first_sentences_regex='^[\s\S]+?\.[\s\S]+?\.'
remove_tags_regex='>[^<>]+?(?=<)'
toc_line_name_regex='>[\s\S]+<'

function get_article_info {
    toc_lines=$(ggrep -P "$toc_lines_regex" "wikipedia_${1}.html")
    toc_line_names=$(echo $toc_lines | ggrep -Po "$toc_line_name_regex" | ggrep -Po "$remove_tags_regex" | tr -d '>' | ggrep -Po '.+')
    readarray -t line_split_arr <<<"$toc_line_names"

    ggrep -Po "$get_paragraphs" "wikipedia_${1}.html" > "wiki_paragraphs.txt"
    first_sentence=$(ggrep -Po "$remove_tags_regex" "wiki_paragraphs.txt" | tr -d '\n' | tr -d '>' | ggrep -Po "$first_sentences_regex")

    index=0
    toc_arr=()
    printf "%s\n\n" "$first_sentence"
    for line in "${line_split_arr[@]}"; do
        new_line=${line//\n+}
        new_line=${new_line// }
        if [  -n "$new_line" ] ; then
            if [[ $line =~ ^[0-9]+([.][0-9]+)?$ ]] ; then
                toc_arr[index]="$line"
            else    
                echo "${toc_arr[index]} $line"
                toc_arr[index]="${toc_arr[index]}; $line"
                index=$(($index+1))
            fi
        fi
    done
    # echo "${toc_arr[@]}"
}

function get_section_info {
    toc_lines=$(ggrep -P "$toc_lines_regex" "wikipedia_${1}.html")
    toc_line_names=$(echo $toc_lines | ggrep -Po "$toc_line_name_regex" | ggrep -Po "$remove_tags_regex" | tr -d '>' | ggrep -Po '.+')
    readarray -t line_split_arr <<<"$toc_line_names"

    index=0
    last_section_num=0
    toc_arr=()
    last_section_num=0
    section_num=0
    printf "%s\n\n" "$first_sentence"

    for line in "${line_split_arr[@]}"; do
        new_line=${line//\n+}
        new_line=${new_line// }
        new_line=${new_line//;}
        if [  -n "$new_line" ] ; then
            if [[ $line =~ ([0-9]|[.]) ]] ; then
                if [ $section_num -ne 0 ] ; then 
                    if [[ $line =~ (${section_num}\w+) ]] ; then 
                        toc_arr[index]="$line"
                    fi
                fi   
                last_section_num=$line          
            else  
                if [ $section_num -ne 0 ] ; then 
                    if [[ $line =~ (${section_num}[\s\S]+) ]] ; then 
                        toc_arr[index]="${toc_arr[index]};$line"
                    fi
                else
                    if [[ $line = "$2" ]] ; then
                        section_num=$last_section_num
                        toc_arr[index]="${section_num};$line"
                        index=$(($index+1))
                    fi
                fi
                # echo "${toc_arr[index]} $line"
            fi
        fi
    done

    for section in "${toc_arr[@]}" ; do
        echo "$section"
        # expr "$section" : '[\s\S]+'
    done
}

function get_wiki {
    curl -L https://en.wikipedia.org/wiki/"${1}" -o "wikipedia_${1}.html"
    if [ $# -eq 1 ] ; then 
        get_article_info "$@"
    elif [ $# -eq 2 ] ; then
        get_section_info "$@"
    else 
        echo "Must provide one or two arguments"
    fi
}

get_wiki walrus References
 
