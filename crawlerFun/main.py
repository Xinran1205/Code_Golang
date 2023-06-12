# This is a sample Python script.
import os
from urllib.parse import urljoin

import requests
from bs4 import BeautifulSoup
from selenium import webdriver
# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.


def crawler_bai():
    head = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko)"
                      " Chrome/112.0.0.0 Safari/537.36 Edg/112.0.1722.68"
    }

    for page in range(1,4,1):
        if page==1:
            response = requests.get(
                "https://search.bilibili.com/all?keyword=%E9%BB%91%E4%B8%9D&from_source=webtop_search&spm_id_from=333.1073&search_source=2",
            headers = head)
        else:
            x = page
            y = 30 * (page - 1)
            s = "https://search.bilibili.com/all?keyword=%%E9%%BB%%91%%E4%%B8%%9D&from_source=webtop_search&spm_id_from=333.1073&search_source=2&page=%d&o=%d"
            s_formatted = s % (x, y)
            response = requests.get(s_formatted, headers=head)

        print(response.url)
        soup = BeautifulSoup(response.content, "html.parser")
        img_tags = soup.find_all("img")
        img_urls = [urljoin(response.url, img["src"]) for img in img_tags]

        count=0
        os.makedirs("F:/crawlerFun/images", exist_ok=True)
        for i, img_url in enumerate(img_urls):
            response = requests.get(img_url)
            with open(f"F:/crawlerFun/images/image_{i}{page}.jpg", "wb") as f:
                f.write(response.content)
                count=count+1
            if(count==5):
                break




# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    crawler_bai()

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
