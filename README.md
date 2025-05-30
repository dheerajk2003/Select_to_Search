# Select to Search (STS)

**Select to Search (STS)** is a lightweight Linux utility inspired by Android's "Circle to Search." It lets users on X11-based Linux systems select any region of the screen and perform a contextual search.

Why X11 only? Because that's what the developer uses. :)

Depending on how the program is run, results are either shown in your default browser (as an HTML page) or via a native desktop notification.

---

## ✨ Features

- Select any screen area to trigger a search.
- Two output modes:
  - **Browser Output:** Default behavior. Opens an HTML file with the search result.
  - **Notification Output:** Run with the `-n` flag to receive the result via desktop notification.
- Lightweight and fast.
- Works on all X11-based Linux desktop environments.

---

## 🛠 Installation

### 1. Clone and Build

```bash
git clone https://github.com/dheerajk2003/Select_to_Search.git
cd Select_to_Search
mkdir build && cd build
cmake ..
make
```

### 📦 Dependencies
Make sure the following packages are installed:

x11

libpng

libcurl

libcjson

libnotify

glib

cmake

Install on Common Distros
Arch Linux / Manjaro:

```
sudo pacman -S libx11 libpng curl cjson libnotify glib2 cmake
```


Debian / Ubuntu / Linux Mint:

```
sudo apt install libx11-dev libpng-dev libcurl4-openssl-dev libcjson-dev libnotify-dev libglib2.0-dev cmake
```


Fedora:

```
sudo dnf install libX11-devel libpng-devel libcurl-devel libcjson-devel libnotify-devel glib2-devel cmake
```


---

### 🔑 Gemini API Key
This tool requires a Gemini API key for accessing the search functionality.

To set it up, add the following line to your **/etc/environment** file:

```
GEMINI_API_KEY=your_api_key
```

Then restart your session or run source /etc/environment.

---

 ### 🚀 Usage
#### 1. Default (HTML in browser):

```
 ./clone_dir/Select_to_Search/build/sts && firefox /home/user/Documents/STS/LatestResponse.html 
 ```

#### 2. Notification mode:

```
 ./clone_dir/Select_to_Search/build/sts -n 
 ```


---

### 📂 Output
HTML Response File:
/home/user/Documents/STS/LatestResponse.html

Notification Text:
Displays the main search result as a popup notification.