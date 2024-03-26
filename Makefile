#Kullman Alexandru, 313CA

build: star_dust
all: star_dust

star_dust : star_dust.c
	gcc star_dust.c -Wall -Wextra -o star_dust
	

clean:
	rm -rf all
