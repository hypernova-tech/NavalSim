

import tkinter as tk
import socket

# UDP mesajını gönderen fonksiyon
def send_udp_message():
    message = entry.get()
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as udp_socket:
        udp_socket.sendto(message.encode('utf-8'), ('127.0.0.1', 12345))  # Burada 127.0.0.1 lokal IP ve 12345 port numarasıdır.

# Enter tuşuna basıldığında tetiklenen fonksiyon
def on_enter(event):
    send_udp_message()

# Tkinter penceresi oluştur
root = tk.Tk()
root.title('UDP Message Sender')

# Entry (Text Field) widget'ı oluştur
entry = tk.Entry(root, width=40)
entry.pack(padx=20, pady=20)

# Enter tuşuna basıldığında `on_enter` fonksiyonunu çağır
entry.bind('<Return>', on_enter)

# Buton widget'ı oluştur
button = tk.Button(root, text='Send', command=send_udp_message)
button.pack(pady=10)

# Tkinter event loop
root.mainloop()