# FuzzumBuildToolOnline  
### Build Fuzzum Engine games for Android — directly from your phone 📱

Compile your **Fuzzum Engine** project into a signed **Android APK** — **no PC, no Termux, no setup required**.  
Powered by **GitHub Actions**, this repository provides a **fully automated cloud build system** that works entirely in your mobile browser.

> ✨ All you need: a GitHub account and a smartphone.

---

## 🇬🇧 English

### 🚀 How to Use (Mobile-Friendly)

#### 1. **Fork this repository**
- Open [github.com/JustOrangeSFM/FuzzumBuildToolOnline](https://github.com/JustOrangeSFM/FuzzumBuildToolOnline) on your phone.
- Tap **Fork** (top-right) → select your account.

#### 2. **Clear the example project**
- Go to the `GameProject/` folder.
- Delete all files and folders inside it (but keep the `GameProject/` folder itself).
  - Tap each item → 🖊️ **Edit** → **Delete this file** → **Commit changes**.

#### 3. **Upload your game**
- Tap **Add file** → **Upload files**.
- Select your **entire game folder** (must include `Game.project`, `Source/`, `Assets/`, etc.).
- Upload it into `GameProject/YourGame/` (create a subfolder with your game’s name).
- ⚠️ Open your `Game.project` file and set:
  ```json
  "EnginePath": "D:/a/FuzzumBuildToolOnline/FuzzumBuildToolOnline/Engine/Engine"
  ```

#### 4. **Configure the build script**
- Open the file: `[BUILD]BuildEngine[Android].bat`
- Update this line to match your game folder name:
  ```bat
  BuildToolCpp.exe build --project=GameProject/YourGame/Game.project --platform=Android
  ```
- Click **Commit changes**.

#### 5. **Start the build**
- Go to the **Actions** tab.
- If prompted, enable workflows.
- Click **Build Android APK** → **Run workflow** → **Run workflow** (green button).
- Wait **1–5 minutes** (depending on your project size).

#### 6. **Download your APK**
- When the workflow finishes successfully (✅ green checkmark), scroll down to **Artifacts**.
- Download `game-apk.zip` → extract it → install the `.apk` on your Android device.
- 🔐 Go to **Android Settings → Apps → Special access → Install unknown apps** and allow your browser.

---

### 📁 Required Project Structure

Your game must follow this layout:

```
YourGame/
├── Game.project
├── Assets/
├── Source/
│   └── ... (your Lua/AngelScript/C++ modules)
└── Plugins/ (optional)
```

> 💡 Make sure `Game.project` is valid and uses correct relative paths.

---

### ⚠️ Notes
- Do **not** commit any `Build/` folders — they are generated automatically.
- The build environment uses:
  - **Android NDK r25b**
  - **JDK 17**
  - **Android SDK API 33**
- `BuildToolCpp.exe` and engine binaries are **pre-included** and **proprietary** (not open source).

---

### 📜 License
- Build scripts and documentation: **MIT License**
- `BuildToolCpp.exe`, engine binaries, and internal tools: **Proprietary — not licensed for redistribution**

Full license: [LICENSE](./LICENSE.txt)

---

### ❓ Need Help?
- Check the **Actions logs** if the build fails.
- Verify your `EnginePath` in `Game.project`.
- Ensure your game folder is placed under `GameProject/`.

> Made with ❤️ for indie developers who want true mobile freedom.  
> **No desktop. No Termux. Just build.**

---

## 🇷🇺 Русский

### 🚀 Как использовать (на телефоне)

#### 1. **Сделайте форк репозитория**
- Откройте [github.com/JustOrangeSFM/FuzzumBuildToolOnline](https://github.com/JustOrangeSFM/FuzzumBuildToolOnline) в браузере телефона.
- Нажмите **Fork** (вверху справа) → выберите свой аккаунт.

#### 2. **Удалите пример проекта**
- Перейдите в папку `GameProject/`.
- Удалите все файлы и папки внутри (саму папку `GameProject/` оставьте).
  - Нажмите на каждый элемент → 🖊️ **Edit** → **Delete this file** → **Commit changes**.

#### 3. **Загрузите свою игру**
- Нажмите **Add file** → **Upload files**.
- Выберите **всю папку вашей игры** (должны быть `Game.project`, `Source/`, `Assets/` и т.д.).
- Загрузите её в `GameProject/ВашаИгра/` (создайте подпапку с названием вашей игры).
- ⚠️ Откройте файл `Game.project` и укажите:
  ```json
  "EnginePath": "D:/a/FuzzumBuildToolOnline/FuzzumBuildToolOnline/Engine/Engine"
  ```

#### 4. **Настройте скрипт сборки**
- Откройте файл: `[BUILD]BuildEngine[Android].bat`
- Измените эту строку, указав имя вашей папки:
  ```bat
  BuildToolCpp.exe build --project=GameProject/ВашаИгра/Game.project --platform=Android
  ```
- Нажмите **Commit changes**.

#### 5. **Запустите сборку**
- Перейдите во вкладку **Actions**.
- Если потребуется, включите workflows.
- Нажмите **Build Android APK** → **Run workflow** → **Run workflow** (зелёная кнопка).
- Подождите **1–5 минут** (зависит от размера проекта).

#### 6. **Скачайте APK**
- После успешной сборки (✅ зелёная галочка), пролистайте вниз до **Artifacts**.
- Скачайте `game-apk.zip` → распакуйте → установите `.apk` на Android.
- 🔐 Зайдите в **Настройки → Приложения → Специальный доступ → Установка неизвестных приложений** и разрешите установку для вашего браузера.

---

### 📁 Требуемая структура проекта

```
ВашаИгра/
├── Game.project
├── Assets/
├── Source/
│   └── ... (ваши модули на Lua/AngelScript/C++)
└── Plugins/ (опционально)
```

> 💡 Убедитесь, что `Game.project` корректен и пути указаны правильно.

---

### ⚠️ Важно
- **НЕ загружайте** папки `Build/` — они создаются автоматически.
- Сборка использует:
  - **Android NDK r25b**
  - **JDK 17**
  - **Android SDK API 33**
- `BuildToolCpp.exe` и бинарники движка — **встроены** и **закрыты** (не open source).

---

### 📜 Лицензия
- Скрипты сборки и документация: **MIT License**
- `BuildToolCpp.exe`, бинарники движка и внутренние инструменты: **Проприетарные — запрещено распространение**

Полный текст лицензии: [LICENSE](./LICENSE)

---

### ❓ Нужна помощь?
- Смотрите **логи в Actions**, если сборка падает.
- Проверьте `EnginePath` в `Game.project`.
- Убедитесь, что папка игры лежит внутри `GameProject/`.

> Сделано с ❤️ для независимых разработчиков, ценящих свободу на мобильных устройствах.  
> **Без компьютера. Без Termux. Просто собери.**
```

---