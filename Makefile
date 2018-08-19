DOCKER_COMPOSE := docker-compose -f ./docker_env/docker-compose.yml
DOCKER_EXEC := docker exec -it
CONTAINER_NAME := review

ps:
	$(DOCKER_COMPOSE) ps

setup: build up review/setup vivliostyle/setup

build:
	$(DOCKER_COMPOSE) build

up:
	$(DOCKER_COMPOSE) up -d

clean: stop rm

stop:
	$(DOCKER_COMPOSE) stop

rm:
	$(DOCKER_COMPOSE) rm -f

attach:
	$(DOCKER_COMPOSE) exec $(CONTAINER_NAME) /bin/bash

# ===================================================
# vivliostyle
# ===================================================

vivliostyle/setup:
	$(DOCKER_COMPOSE) exec $(CONTAINER_NAME) /bin/bash -ci "./docker_env/review/vivliostyle_setup.sh"

vivliostyle/%:
	$(DOCKER_COMPOSE) exec $(CONTAINER_NAME) /bin/bash -ci "npm run vivliostyle/$*"

# ===================================================
# review
# ===================================================

review/setup:
	$(DOCKER_COMPOSE) exec $(CONTAINER_NAME) /bin/bash -ci "./setup.sh"

review/%:
	$(DOCKER_COMPOSE) exec $(CONTAINER_NAME) /bin/bash -ci "npm run review/$*"

# ===================================================
# npm run {command}
# ===================================================

npm/run/%:
	$(DOCKER_COMPOSE) exec $(CONTAINER_NAME) /bin/bash -ci "npm run $*"

# ===================================================
# [css] pdf & browser
# ===================================================

PDF := ./articles/book.pdf
VIVLIOSTYLE_VIEWER_URL := http://0.0.0.0:8000/docker_env/review/vivliostyle/vivliostyle-js-2017.6/viewer/vivliostyle-viewer.html
HTML_URL := http://127.0.0.1:8989/book.html
PAGE_FORMAT := A5

server: review/up vivliostyle/up
server/kill: review/kill vivliostyle/kill

build/pdf: npm/run/css
	$(MAKE) server
	$(DOCKER_COMPOSE) exec $(CONTAINER_NAME) /bin/bash -ci "node docker_env/scripts/pdf.js $(PDF) $(VIVLIOSTYLE_VIEWER_URL)#x=$(HTML_URL) $(PAGE_FORMAT)"
	open $(PDF)

build/browser: npm/run/css
	$(MAKE) server
	open $(VIVLIOSTYLE_VIEWER_URL)#x=$(HTML_URL)

css/pdf:
	$(MAKE) -j build/pdf

css/browser:
	$(MAKE) -j build/browser
