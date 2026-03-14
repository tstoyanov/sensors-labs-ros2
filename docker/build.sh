#!/bin/bash

set -e

docker compose build --no-cache

echo "Docker image built successfully."
