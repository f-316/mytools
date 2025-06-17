# マルチキャスト版
import cv2
import asyncio
import websockets

connected_clients = set()

async def stream():
    cap = cv2.VideoCapture(0)
    try:
        while True:
            ret, frame = cap.read()
            if not ret:
                continue
            _, buffer = cv2.imencode('.jpg', frame)
            data = buffer.tobytes()

            # 各クライアントに送信
            disconnected = set()
            for client in connected_clients:
                try:
                    await client.send(data)
                except websockets.exceptions.ConnectionClosed:
                    disconnected.add(client)

            # 切断されたクライアントを削除
            connected_clients.difference_update(disconnected)

            await asyncio.sleep(1 / 30)  # 約30fps
    finally:
        cap.release()

async def handler(websocket):
    connected_clients.add(websocket)
    print(f"Client connected: {websocket.remote_address}")
    try:
        await websocket.wait_closed()
    finally:
        connected_clients.remove(websocket)
        print(f"Client disconnected: {websocket.remote_address}")

async def main():
    print("WebSocket server started on ws://0.0.0.0:8080")
    async with websockets.serve(handler, "0.0.0.0", 8080):
        await stream()  # 1スレッドで共有カメラをブロードキャスト

asyncio.run(main())
