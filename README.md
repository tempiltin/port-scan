
---

# 🔍 C++ Port Scanner (Kali Linux uchun)

Bu dastur berilgan IP manzildagi ochiq portlarni aniqlaydi. Asosan **TCP portlar** orqali **connect()** funksiyasidan foydalanadi. Juda sodda, lekin samarali port scanner.

---

## 🛠 Kerakli kutubxonalar

```cpp
#include <iostream>             // Ekranga chiqish va foydalanuvchidan ma'lumot olish uchun
#include <sys/socket.h>         // socket(), connect() kabi tarmoq funksiyalari uchun
#include <netinet/in.h>         // sockaddr_in strukturasi uchun
#include <arpa/inet.h>          // inet_pton() funksiyasi uchun (IP ni binar formatga o‘tkazish)
#include <unistd.h>             // close() funksiyasi uchun (socketni yopish)
```

```cpp
using namespace std;  // std::cout o‘rniga faqat cout ishlatish uchun
```

---

## 🔐 Port ochiqligini tekshiruvchi funksiya

```cpp
bool is_port_open(const string& ip, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return false;
```

- **socket()** – yangi TCP soket yaratadi.
- Agar **sockfd < 0** bo‘lsa, socket yaratishda xatolik bo‘lgan, demak portni tekshirib bo‘lmaydi.

---

```cpp
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
```

- `sockaddr_in` – bu IP va portni o‘zida tutuvchi strukturadir.
- **AF_INET** – IPv4 ishlatilayotganini bildiradi.
- **htons(port)** – portni tarmoq formatiga o‘tkazadi.
- **inet_pton()** – IP manzilni stringdan binar formatga o‘tkazadi.

---

```cpp
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;  // 100 millisekund (0.1 soniya)
```

- **timeout** – har bir portga qancha kutish kerakligini belgilaydi. Juda uzoq kutmaslik uchun 100ms.

---

```cpp
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
```

- Bu ikki qator **socketga vaqt cheklovi qo‘yadi**, shunda u juda uzoq kutmaydi.
- Bu orqali port ochiq yoki yopiq ekani tez aniqlanadi.

---

```cpp
    int result = connect(sockfd, (sockaddr*)&addr, sizeof(addr));
    close(sockfd);

    return result == 0;
}
```

- **connect()** – belgilangan IP va portga ulanib ko‘radi.
- Agar **connect() == 0**, demak port **ochiq**.
- Har bir tekshiruvdan so‘ng socketni **yopish** zarur (`close()`).

---

## 🧠 Main funksiyasi

```cpp
int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Foydalanish: " << argv[0] << " <IP manzil>\n";
        return 1;
    }
```

- Dastur **faqat bitta argument** – IP manzilni qabul qiladi.
- Agar foydalanuvchi noto‘g‘ri foydalansa, yordamchi xabar chiqaradi.

---

```cpp
    string ip = argv[1];
    cout << "Port skanerlash boshlanmoqda: " << ip << endl;
```

- Foydalanuvchidan olingan IP manzilni `ip` o‘zgaruvchisiga saqlaydi.

---

### 🔁 Portlar bo‘yicha tekshirish

```cpp
    for (int port = 20; port <= 1024; ++port) {
        if (is_port_open(ip, port)) {
            cout << "Ochiq port topildi: " << port << endl;
        }
    }
```

- **20 dan 1024 gacha** bo‘lgan portlar tekshiriladi (bu diapazon "well-known ports").
- Agar port ochiq bo‘lsa – ekranga chiqariladi.

---

```cpp
    cout << "Skanerlash tugadi.\n";
    return 0;
}
```

- Tekshiruv tugagach, xabar chiqariladi.

---

## 🧪 Dasturdan foydalanish

```bash
g++ -o port-scan port-scan.cpp
./port-scan 127.0.0.1
```

---

## 📋 Misol natija:

```bash
Port skanerlash boshlanmoqda: 127.0.0.1
Ochiq port topildi: 22
Ochiq port topildi: 80
Skanerlash tugadi.
```

---

## 🔐 Qo‘llanilishi (Cybersecurity nuqtayi nazaridan):

- **Penetratsion test** uchun ochiq portlarni topish.
- **Zaruriy xavfsizlik choralarini ko‘rish** uchun tarmoqlarda skanerlash.
- **Yopilmagan portlar orqali potentsial zaifliklarni** aniqlash.

---

Xohlasangiz, bu dasturga:
- GUI qo‘shish
- Port oralig‘ini foydalanuvchi belgilashi
- Multi-threading yordamida tezlashtirish
kabi imkoniyatlarni ham qo‘shishimiz mumkin.
