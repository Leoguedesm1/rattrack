DOCKER_IMAGE = adnriv/opencv

SOURCE_DIR = ../QT
BUILD_DIR = ./build
DOCKER_DIR = /builds	

DOCKER_DISPLAY_ARGS = --env="DISPLAY" --net=host --env="QT_X11_NO_MITSHM=1" \
											--volume="${HOME}/.Xauthority:/root/.Xauthority:rw" 
											# --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw"
DOCKER_VOLUMES = --volume=$(shell pwd):$(DOCKER_DIR) --workdir=$(DOCKER_DIR)	

DOCKER_RUN = docker run -it --rm $(DOCKER_RUN_ARGS) $(DOCKER_VOLUMES) $(DOCKER_IMAGE) bash -c "qmake $(SOURCE_DIR); make"

all: rattrack

rattrack: build

execute: build
	docker run -it --rm $(DOCKER_DISPLAY_ARGS) $(DOCKER_RUN_ARGS) $(DOCKER_VOLUMES) $(DOCKER_IMAGE) $(BUILD_DIR)/rattrack; \
	status=$$?

build:
	# Create the build directory
	mkdir $(BUILD_DIR) || true
	# Build inside the docker
	docker run -it --rm $(DOCKER_RUN_ARGS) $(DOCKER_VOLUMES) $(DOCKER_IMAGE) bash -c "cd $(BUILD_DIR); qmake $(SOURCE_DIR); make"; \
	 status=$$?

clean:
	rm -rf build/

.PHONY: all build execute clean
