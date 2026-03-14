#!/bin/bash

#set -e

docker compose run --name ros2 --rm --service-ports robotics_service bash
