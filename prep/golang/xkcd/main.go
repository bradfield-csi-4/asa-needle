// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

//!+

package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
	"strconv"
	"strings"
)

func main() {
	if len(os.Args) > 0 {
		res := searchIndex(os.Args[1])
		result, err := json.MarshalIndent(res, "	", "	")
		if err != nil {
			fmt.Println(err)
		}
		fmt.Printf("%s\n", string(result))
	}
}

func searchIndex(term string) []ComicSearchTermResult {
	jsonFile, err := os.Open("comic_index.json")
	if err != nil {
		fmt.Println(err)
	}
	fmt.Println("Successfully Opened comic_index.json")
	defer jsonFile.Close()
	byteValue, _ := ioutil.ReadAll(jsonFile)
	var index []ComicResult
	json.Unmarshal(byteValue, &index)

	var results []ComicSearchTermResult

	for _, value := range index {
		if strings.Contains(value.Transcript, term) {
			results = append(results, ComicSearchTermResult{
				Transcript: value.Transcript,
				Num:        value.Num,
			})
		}
	}
	return results
}

func buildIndex() (total_count int, err error) {
	cur_comic_num := 1
	var comics []ComicResult
	for {
		if cur_comic_num != 404 {
			fmt.Printf("comic num: %d\n", cur_comic_num)
			var comic, cur_err = GetComic(cur_comic_num)
			if cur_err != nil {
				fmt.Println(cur_err)
				break
			}
			comics = append(comics, *comic)
		}
		cur_comic_num += 1
	}
	res, mi_err := json.MarshalIndent(comics, "", "")
	if mi_err != nil {
		return 0, mi_err
	}
	os_err := os.WriteFile("comic_index.json", res, 0644)
	if os_err != nil {
		return 0, os_err
	}
	return cur_comic_num, nil
}

func GetComic(comic_num int) (*ComicResult, error) {
	url := fmt.Sprintf("https://xkcd.com/%s/info.0.json", strconv.Itoa(comic_num))
	resp, err := http.Get(url)
	if err != nil {
		fmt.Printf("there was an error getting the comic")
		return nil, err
	}
	if resp.StatusCode != http.StatusOK {
		fmt.Printf("%s\n", "bad status code")
		resp.Body.Close()
		return nil, fmt.Errorf(resp.Status)
	} else {
		var result ComicResult
		if err := json.NewDecoder(resp.Body).Decode(&result); err != nil {
			resp.Body.Close()
			return nil, err
		}
		resp.Body.Close()
		return &result, nil
	}
}

type ComicIndexBuildResult struct {
	TotalCount int `json:"total_count"`
	Items      []*ComicResult
}

type ComicResult struct {
	Month      string
	Num        int
	Link       string
	Year       string
	News       string
	SafeTitle  string `json:"safe_title"`
	Transcript string
	Alt        string
	Img        string
	Title      string
	Day        string
}

type ComicSearchTermResult struct {
	Transcript string
	Num        int
}
