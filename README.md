# SOCKET
# SCTP Sunucusu ve İstemci Örneği

## Genel Bakış

Bu depo, bir sunucu ile bir veya daha fazla istemci arasında iletişim kurmak için SCTP (Stream Control Transmission Protocol) kullanımını gösteren basit C programları içerir. SCTP, TCP ve UDP'nin bazı özelliklerini birleştirerek, mesaj tabanlı iletişim ve çoklu akış gibi avantajlar sunar.

Depo şu bileşenleri içerir:
- `server.c`: Birden fazla istemci bağlantısını eş zamanlı olarak yönetebilen SCTP sunucu kodu.
- `client.c`: Sunucuya bağlantı kurarak mesaj gönderen SCTP istemci kodu.

## Gereksinimler

Bu programları derlemek ve çalıştırmak için aşağıdakilere ihtiyacınız olacak:
- Bir C derleyicisi (örneğin, GCC)
- SCTP'yi destekleyen bir işletim sistemi (örneğin, Linux)
- Terminal veya komut istemcisini kullanma temel bilgisi

## Kullanım

### Sunucu

1. Sunucu kodunu derle:
   gcc server.c -o server -lpthread -lsctp
2. Sunucuyu başlat:
./server
3. Sunucu, belirtilen IP adresi ve port üzerinden gelen SCTP bağlantılarını dinleyecektir.

### İstemci

1. İstemci kodunu derle:
gcc client.c -o client -lpthread -lsctp


2. İstemciyi başlat:
./client

3. İstemci terminaline mesajlar girin. Bu mesajlar SCTP üzerinden sunucuya gönderilecektir.

4. İstemciyi kapatmak için "exit" yazıp Enter tuşuna basın.

## Notlar

- Sunucu, aynı anda birden fazla istemci bağlantısını yönetebilir ve her bir istemci ayrı bir iş parçası tarafından yönetilir.
- İstemci, kullanıcıların etkileşimli olarak sunucuya mesaj göndermelerine izin verir.
- Sunucu ve istemci, güvenilir ve mesaj tabanlı veri iletişimi için SCTP'yi kullanır.

## Yazar

MERT YÜKSEL

Herhangi bir sorunuz veya öneriniz varsa lütfen iletişime geçmekten çekinmeyin.
