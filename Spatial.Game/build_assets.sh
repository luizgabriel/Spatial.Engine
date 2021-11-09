#!/bin/zsh
FILAMESH_PATH=../out/build/debug/bin/filamesh

rm meshes/*.filamesh
${FILAMESH_PATH} meshes/plane.obj meshes/plane.filamesh
${FILAMESH_PATH} meshes/cube.obj meshes/cube.filamesh
${FILAMESH_PATH} meshes/sphere.obj meshes/sphere.filamesh
${FILAMESH_PATH} meshes/cylinder.obj meshes/cylinder.filamesh