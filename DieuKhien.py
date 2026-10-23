import socket
import time
ESP32_IP = "192.168.4.1" 
PORT = 4444 

def main():
    print(f"Dang tim kiem xe tai: {ESP32_IP}:{PORT}...")
    try:
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect((ESP32_IP, PORT))
        print("Da ket noi xe thanh cong.\n")

        while True:
            lenh_dieu_khien = "PID: Kp=15.5, Ki=0, Kd=1.2"
            client.sendall(lenh_dieu_khien.encode('utf-8'))
            du_lieu_nhan = client.recv(1024)
            if not du_lieu_nhan:
                print("Xe da ngat ket noi.")
                break
            
            print(f"Nhan tu xe: {du_lieu_nhan.decode('utf-8').strip()}")
            time.sleep(0.5) 
            
    except Exception as e:
        print("Loi mang:", e)
    finally:
        client.close()

if __name__ == "__main__":
    main()