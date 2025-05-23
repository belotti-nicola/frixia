#!/bin/bash

# === CONFIG ===
SERVER_IP="127.0.0.1"
SERVER_PORT=9600
LOCAL_PORT=6000

# FINS REQUEST to HEX 
FINS_HEX="8000020000000000000000000101820000000001"

# HEX to Binary
REQUEST_BIN=$(echo "$FINS_HEX" | xxd -r -p)

echo "[*] Sending FINS to $SERVER_IP:$SERVER_PORT (from local port: $LOCAL_PORT)"
echo "$REQUEST_BIN" | nc -u -p "$LOCAL_PORT" -w 3 "$SERVER_IP" "$SERVER_PORT" | xxd
