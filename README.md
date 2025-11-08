---

# FuzzumBuildToolOnline

Compile your **FuzzumEngine** game project into an Android APK — **entirely from your phone**, with **no computer required**.

This repository provides a fully automated cloud-based build system using **GitHub Actions**. Just fork, upload your project, and get a ready-to-install APK in minutes.

> ✨ Works on **Android** — all you need is a browser.

---

## 🚀 How to Use (Mobile-Friendly)

### 1. **Fork this repository**
- Open [github.com/JustOrangeSFM/FuzzumBuildToolOnline](https://github.com/JustOrangeSFM/FuzzumBuildToolOnline) in your mobile browser.
- Tap **Fork** (top-right).
- Select your account.

### 2. **Clear the example project**
- Go to `GameProject/`
- Delete everything inside (keep the folder itself).
  - Tap each file/folder → 🖊️ **Edit** → **Delete file** → **Commit**.

### 3. **Upload your game**
- Tap **Add file** → **Upload files**.
- Select your entire game project (must include your `.project` file and `Source/` folder and etc.).
- Ensure it’s placed under `GameProject/YourGame/`.

### 4. **Configure the build script**
- Open `[BUILD]BuildEngine[Android].bat`
- Edit the line:
  ```bat
  BuildToolCpp.exe build --project=GameProject/YourGame/Game.project --platform=Android
  ```
- Commit the change.

### 5. **Start the build**
- Go to **Actions** → enable workflows if prompted.
- Select **Build Android APK** → **Run workflow**.
- Wait 1–3 minutes **(depends on the size of your project)**

### 6. **Download your APK**
- When the workflow succeeds (green checkmark), scroll down to **Artifacts**.
- Download `game-apk.zip` → extract → install `*.apk` on your Android device.

> 🔐 Allow **installation from unknown sources** in Android settings.

---

## 📁 Project Structure Requirements

Your game must follow the standard FuzzumEngine layout:

```
YourGame/
├── Game.project
├── Assets/
├── Source/
│   ├── Core/
│   └── ...
└── Plugins/ (optional)
```

> 💡 Your `Game.project` must be valid and reference correct modules.

---

## ⚠️ Notes

- Do **not** commit `ndk/`, `BuildSettings.json`, or build outputs — they’re generated automatically.
- The build uses **NDK r25b**, **JDK 17**, and **Android SDK 33**.
- APK is built in **debug mode** (for quick iteration). For release, modify the workflow.

---

## 🛠️ Need Help?

- Ensure your `.project` path is correct.
- Check **Actions logs** if the build fails.
- Make sure `BuildToolCpp.exe` and `[BUILD]BuildEngine[Android].bat` are in the repo root (they are by default).

---

Made with ❤️ for mobile game developers who want freedom from the desktop.

**No Termux. No setup. Just build.**
