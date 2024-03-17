#!/usr/bin/bash

docker compose up -d
xhost +local:
docker compose exec gcc bash
xhost -local:
