CPy_tutorial
==
Embedded python in c++

* OS: Windows
* IDE: Visual Studio 2017

## 環境設置
### Python 安裝
* 安裝 Python
    先確認電腦本身有沒有裝 Python，只有 Anaconda 是不行的。若沒有，建議從 Python 官方下載 **Windows x86-64 executable installer**: [連結點此](https://www.python.org/downloads/windows/)
    :::warning
    Python 的位元數(x32 or x64)須和 VC 相同
    :::
* 使用 cmd(命令提示字元) 執行 `python` 或 `pip`
    要在 cmd 執行 `python` 或 `pip` 需要將執行檔所在的資料夾加入**環境變數**，步驟如下：
    1. **開啟"系統"**
        打開`控制台 -> 系統`
        或
        打開 cmd，輸入`control system`
        
    3. **開啟"環境變數"**
        點選`系統`頁面左邊`進階系統設定 -> 環境變數`
        
        ![](https://i.imgur.com/ujz1pmc.png)
        
        ![](https://i.imgur.com/rBoMuPX.png)

    4. **設定"環境變數"**
        找到 Path 的環境變數，編輯->新增 `python.exe` 與 `pip.exe` 所在的資料夾路徑，以預設安裝路徑為例:
        ```=
        C:\Users\user\AppData\Local\Programs\Python\Python36
        C:\Users\user\AppData\Local\Programs\Python\Python36\Scripts
        ```
        
        ![](https://i.imgur.com/bFIyQsX.png)
        
        ![](https://i.imgur.com/WA3DIEU.png)

* 安裝 Python 函式庫
    打開**命令提示字元 (cmd)**，並輸入下列指令
    ```shell
    pip install numpy
    ```
* 設定供 Visual Studio 使用的環境變數
    依照先前教過的方法開啟環境變數頁面
    在環境變數中新增 `PYTHONPATH` 變數
    輸入 .dll檔, .h檔, .lib檔的所在位置 **(安裝路徑或版本不同請自行更改)**
    ```=
    C:\Users\user\AppData\Local\Programs\Python\Python36
    C:\Users\user\AppData\Local\Programs\Python\Python36\include
    C:\Users\user\AppData\Local\Programs\Python\Python36\libs
    C:\Users\user\AppData\Local\Programs\Python\Python36\Lib\site-packages\numpy\core\include
    ```
    
    ![](https://i.imgur.com/ItuAfGC.png)

    ![](https://i.imgur.com/bicShJ2.png)
### 設定 Visual Studio 專案屬性
* 設定 .dll 檔位置
    在 ```方案右鍵 > 組態屬性 > 偵錯 > 環境``` 輸入```PATH=$(PYTHONPATH)```
    ![](https://i.imgur.com/kidKKBp.png)
* 設定 .h 檔位置
    在 ```方案右鍵 > 組態屬性 > VC++目錄 > lnclude目錄``` 輸入 ```$(PYTHONPATH);```
* 設定 .lib 檔位置
    在 ```方案右鍵 > 組態屬性 > VC++目錄 > 程式庫目錄``` 輸入 ```$(PYTHONPATH);```
    ![](https://i.imgur.com/yxTy1z2.png)
