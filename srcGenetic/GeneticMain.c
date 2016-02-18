/* This file is unfinished interface to genetic alghoritm implementation
 * There is succesful example of raising SDL_USEREVENT from separate thread
 * and receive this event in Screen_mevent of main thread.
 * This can be the best way to access interface widgets from another thread.
 *
 *
 *      GeneticMain.c - this file is part of SDL_Widgets
 *
 *      Copyright (C) 2013 Dariusz Mikołajczuk <radomik(at)gmail(dot)com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License along
 *      with this program; if not, write to the Free Software Foundation, Inc.,
 *      51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "../srcCommon/StdDefinitions.h"
#include "../srcCommon/Types.h"
#include "../srcCommon/Static.h"
#include "../srcCommon/Screen.h"
#include "../srcCommon/FileBrowsePanel.h"
#include "../srcWidgets/Widget.h"
#include "../srcWidgets/Image.h"
#include "../srcWidgets/Button.h"
#include "../srcWidgets/TextBlock.h"
#include "../srcWidgets/Label.h"
#include "../srcWidgets/LabelImage.h"
#include "../srcWidgets/ButtonImage.h"
#include "../srcWidgets/Rectangle.h"
#include "../srcWidgets/srcContainer/StackList.h"
#include "../srcWidgets/srcContainer/StackListX.h"
#include "../srcWidgets/srcContainer/StackListY.h"
#include "../srcWidgets/srcContainer/Container.h"
#include "../srcWidgets/srcContainer/Grid.h"

/* SECTION USER INTERFACE (variables) */
/* Count of each type of widget, can't be lower than needed */
#define cIMAGE			(10)			// Image count
#define cBUTTON 		(8)
#define cTEXTBLOCK 		(0)
#define cLABEL 			(7)
#define cLABELIMAGE 	(3)
#define cBUTTONIMAGE 	(20)
#define cRECTANGLE 		(10)
#define cSTACKLISTX		(10)
#define	cSTACKLISTY		(10)
#define	cGRID			(1)
static usint c_image=0, c_button=0, c_textblock=0, c_label=0, c_labelimage=0, 
			 c_buttonimage = 0, c_rectangle=0, c_stacklistX=0, c_stacklistY=0, c_grid=0;
static Image 		*image;
static TextBlock 	*text_block;
static Button 		*button;
static Label 		*label;
static LabelImage 	*labelimage;
static ButtonImage  *buttonimage;
static Rectangle    *rectangle;
static StackListX	*stacklistX;
static StackListY	*stacklistY;
static Grid			*grid;
static Screen		sc;
static bool 		is_inited = false;


/* SECTION callback functions and program logic */
#define POPULATION_SIZE		4

usint temp_POPULATION_SIZE, population2_size;
bool mut_after_crossing = true;
usint max_iter = 100;
double delta = 0.001;	// tu niewykorzystywana (maksymalny próg błędu)

uchar max_value = 32;

uchar percent_mut = 10;
uchar percent_cross = 80;

uchar chromosom_length = 5;


// Tylko uchar bo argumenty z zakresu 0...31 , funkcja f(x) = x + 4
uchar population1[POPULATION_SIZE];		// populacja rodziców (bazowa)
uchar population2[POPULATION_SIZE];		// populacja potomna (tymczasowa)

uchar population[2][POPULATION_SIZE];	// dwa wiersze, populacja raz w jednym wierszu raz w drugim
										// aktualna populacja określana zmienną current_p = {0,1}
uchar current_p = 0;
										
uchar function_value[POPULATION_SIZE];	// wartość funckji dla każdego osobnika w populacji



static usint n, m, k;
static uchar p, mother, father, daughter, son;

//~ 10101011
//~ 00010010
//~ 10111001
void perform_mutation(uchar *tab_chromosoms) {
	// Przeprowadź mutację
	for (n = 0; n < POPULATION_SIZE; n++) {		// dla każdego osobnika
		if ((rand() % 101) <= percent_mut) {	// jeśli zachodzi prawdopodop. mutacji
			k = rand() % chromosom_length;		// losuj pozycję zmiany
			p = (1 << k);
			tab_chromosoms[n] ^= p;
		}
	}
}

void genetic_handler() {
	// Wylosuj początkową populacje
	for (n = 0; n < POPULATION_SIZE; n++) 	// losuj początkową populację (zakres 0...max_value-1)
		population[current_p][n] = rand() % max_value;
		
	usint iter = 0;
	while (1) {
		/* Określ wartość funkcji y = x + 4 dla każdego osobnika w populacji 
		 * x - równy wartości chromosomu danego osobnika */
		for (n = 0; n < POPULATION_SIZE; n++)
			function_value[n] = population[current_p][n] + 4;
		
		/* Sprawdź czy należy zakończyć algorytm, pętla kończy się
		 * dla iteracji (iter == max_iter)
		 * Pętla iteruje w nieskończoność dla max_iter == 0 */
		iter++; if ((max_iter) && (iter == max_iter)) break;
		
		/* Jeśli mut_after_crossing == false przeprowadź mutacje przed krzyżowaniem */
		if (! mut_after_crossing)
			perform_mutation(population[current_p]);
		
		
		/* Przeprowadź krzyżowanie */
		temp_POPULATION_SIZE = POPULATION_SIZE;		// wielkość populacji rodziców (będzie się zmniejszać)
		population2_size     = 0;					// wielkość populacji dzieci (będzie się zwiększać)
													
		
		for (n = 0; n < POPULATION_SIZE; n++) {		  // dla każdego osobnika w populacji rodzicielskiej
			
			if ((population[current_p][n] != 0xFF) && // jeżeli osobnik n nie opuścił populacji
				(temp_POPULATION_SIZE > 1) &&		  // jeżeli w populacji rodziców pozostał
													  // ktoś jeszcze oprócz osobnika n
				((rand() % 101) <= percent_cross)) 	  // jeżeli zachodzi prawdopodobieństwo krzyżowania
			{
				father = population1[n];		// nowy ojciec
				population1[n] = 0xFF;			// opuszcza populację
				
				
			
				for (m = n + 1; m < POPULATION_SIZE; m++) {	// znajdź parę dla osobnika n
					
				}
				
			}
		}
		
		
		
		if (mut_after_crossing) {
			
		}
		
	} 
}

static SDL_mutex *runMutex;
static SDL_Thread *thread;
static bool run;

int thread_main(void *data) {
	usint k=0;
	SDL_Event event;
	event.user.type = SDL_USEREVENT;
	event.user.code = 1234;
	while (1) {
		fprintf(stderr, "k=%hu\n", k++);
		SDL_PushEvent(&event);
		
		SDL_LockMutex(runMutex);
		if (! run) {
			SDL_UnlockMutex(runMutex);
			fprintf(stderr, "Thread: finish\n");
			return 0;
		}
		else {
			SDL_UnlockMutex(runMutex);
			SDL_Delay(2000);
		}
	}
}

void genetic_user_event(Screen *screen, SDL_UserEvent *uevent) {
	fprintf(stderr, "USER EVENT\n");
	//fprintf(stderr, "USER EVENT %d\n", uevent->code);
}

void button_start(Widget *sender, Screen *screen) {
	SDL_LockMutex(runMutex);
	if (run) {
		SDL_UnlockMutex(runMutex);
		fprintf(stderr, "Thread already started\n");
		return;
	}
	else {
		SDL_UnlockMutex(runMutex);
		fprintf(stderr, "Starting thread\n");
	}
	run = true;
	
	thread = SDL_CreateThread(thread_main, NULL);
	if (! thread) {
		fprintf(stderr, "Failed to create thread: %s\n", SDL_GetError());
		run = false;
		return;
	}
	
}

static bool is_inited, is_running;

struct chromosom {
	uchar value;
	uchar func_value;
	
	StackListX sli;
};
typedef struct chromosom chromosom;

chromosom *populacja;
usint popul_size;

void utworz_populacje() {
	if ((popul_size < POPUL_SIZE) || (! populacja)) {
		
	}
}

void losuj_populacje() {
	utworz_populacje();
	// Wylosuj chromosomy
	
	// Utwórz odpowiednie obiekty
	for (n = 0; n < POPUL_SIZE; n++) {
		
	}
	
	
	is_inited = true;
}

void resetuj() {
	
	is_inited = is_running = false;
}


int genetic_thread(void *data) {
	SDL_Event event;
	event.user.type = SDL_USEREVENT;
	
	while (1) {
		/* Sleep on cond variable - cond + mut */
		
		
		switch (gen_command) {
			case LOSUJ:
				losuj();
				event.user.code = REDRAW;
				SDL_PushEvent(&event);
				break;
			case RESETUJ:
				resetuj();
				event.user.code = CLEAR;
				SDL_PushEvent(&event);
				break;
		}
	
	}
}

void user_event_handle(Screen *screen, SDL_Event *uevent) {
	switch (uevent->code) {
		case REDRAW:
			// Draw background and set of chromosoms
			
			break;
		
		case CLEAR:
			
			break;
	}
}

void button_stop(Widget *sender, Screen *screen) {
	SDL_LockMutex(runMutex);
	if (run) {
		fprintf(stderr, "Main: Stopping thread\n");
		run = false;
		SDL_UnlockMutex(runMutex);
		SDL_WaitThread(thread, NULL);
		fprintf(stderr, "Main: Thread stopped\n");
	}
	else {
		SDL_UnlockMutex(runMutex);
		fprintf(stderr, "Thread not started\n");
	}
}

void gmscreen_toogled_drag_on(Screen *screen) {
	sprintf(label[0].text_block.text+15, "%s", (screen->drag_on) ? " WŁĄCZONE" : "WYŁĄCZONE");
	Label_refresh(&(label[0]));
	Widget_draw(&(label[0].widget), screen, true);
}



/* SECTION USER INTERFACE (creation) */
int GeneticMain_createInterface() {
	image 			= (cIMAGE > 0) 		? malloc(sizeof(Image)*cIMAGE) 				: NULL;
	text_block 		= (cTEXTBLOCK > 0)	? malloc(sizeof(TextBlock)*cTEXTBLOCK) 		: NULL;
	button 			= (cBUTTON > 0) 	? malloc(sizeof(Button)*cBUTTON) 			: NULL;
	label 			= (cLABEL > 0) 		? malloc(sizeof(Label)*cLABEL) 				: NULL;
	labelimage 		= (cLABELIMAGE > 0) ? malloc(sizeof(LabelImage)*cLABELIMAGE) 	: NULL;
	buttonimage 	= (cBUTTONIMAGE > 0)? malloc(sizeof(ButtonImage)*cBUTTONIMAGE)  : NULL;
	rectangle   	= (cRECTANGLE > 0)  ? malloc(sizeof(Rectangle)*cRECTANGLE)      : NULL;
	stacklistX		= (cSTACKLISTX > 0)	? malloc(sizeof(StackListX)*cSTACKLISTX)	: NULL;
	stacklistY		= (cSTACKLISTY > 0)	? malloc(sizeof(StackListY)*cSTACKLISTY)	: NULL;
	grid			= (cGRID > 0)		? malloc(sizeof(Grid)*cGRID)				: NULL;
	
	TTF_Font *font2 = Static_getFont("fonts/ariblk.ttf", 20);
	if (! font2) {
		fprintf(stderr, "GeneticMain_createInterface:GeneticMain failed to load font \"fonts/ariblk.ttf\"\n");
		return 1;
	}
	Screen_setBackground(&sc, "img/25600-colorful-high-resolution-background.jpg");
	Image_scale(sc.background, 0.989583333, 0.833333333, 1);
	usint slix_off, sliy_off, butimg_off, label_off, i, j, k, f, rect_off;
	StackList *sli;
	ButtonImage *butimg;
	Button *but;
	Widget *wt;
	Container *cont;
	
	{/*!! Screen widgets section (ZERO SECTION) these widgets aren't placed in containers !!*/
		// Create buttonimage[0] (app exit) and add it to screen
		ButtonImage_new(&(buttonimage[c_buttonimage]), sc.screen, "img/application-exit-5.png", true);
		butimg = &(buttonimage[c_buttonimage]);
		wt = &(butimg->widget);
		wt->click_handler = Screen_buttonExitClicked;
		ButtonImage_applyDefaultStyle(butimg, 1750, 14, 20, 20);
		ButtonImage_setFixedWidth(butimg, true);
		Screen_addWidget(&sc, wt);
		c_buttonimage++;
		
		// Create label[0] (drag on/off indicator) and add it to screen
		Label_new(&(label[c_label]), sc.screen, "Przeciąganie:              ", false);
		sprintf(label[c_label].text_block.text+15, "%s", (sc.drag_on) ? " WŁĄCZONE" : "WYŁĄCZONE");
		Label_setFont(&(label[c_label]), font2);
		Label_setPadding(&(label[c_label]), 10, 5);
		Label_setBorder(&(label[c_label]), 4, 0xFF006C);
		Label_setFontColor(&(label[c_label]), 0x003FE1);
		Label_setBgColor(&(label[c_label]), 0xFFF9A7);
		wt = &(label[c_label].widget);
		wt->pos.x = 1;
		wt->pos.y = 950;
		Label_refresh(&(label[c_label]));
		label[c_label].fixed_width = true;
		Screen_addWidget(&sc, wt);
		c_label++;
	}
	
	{
		Button_new(&(button[c_button]), sc.screen, "Start", true);
		but = &(button[c_button]);
		wt = &(but->widget);
		wt->click_handler = button_start;
		Button_applyDefaultStyle(but, 500, 500, font2, 5, 5);
		Button_setFixedWidth(but, true);
		Screen_addWidget(&sc, wt);
		c_button++;
		
		Button_new(&(button[c_button]), sc.screen, "Stop", true);
		but = &(button[c_button]);
		wt = &(but->widget);
		wt->click_handler = button_stop;
		Button_applyDefaultStyle(but, 500, 600, font2, 5, 5);
		Button_setFixedWidth(but, true);
		Screen_addWidget(&sc, wt);
		c_button++;
	}
	return 0;
}

int GeneticMain_Initialize(SDL_Surface *screen) {
	if (is_inited) return 0;	// already initialized
	
	Screen_init(&sc, screen, 0);
	/*sc.toogle_drag_on = gmscreen_toogled_drag_on;	// add handler to drag on toogle events*/
	sc.user_event = genetic_user_event;	// SDL_USEREVENT handler
	
	/* Create user interface */
	if (GeneticMain_createInterface()) {
		fprintf(stderr, "GeneticMain_createInterface:GeneticMain failed\n");
		return 1;
	}
	
	fprintf(stderr, "GeneticMain_Initialize > sc.c_widget=%u\n", sc.c_widget);
	// Perform static global variables initialization
	runMutex = SDL_CreateMutex();
	thread = NULL;
	run = false;
	populacja = NULL;
	
	
	// common
	is_inited = true;
	return 0;
}



int GeneticMain_Main(SDL_Surface *screen) {
	if (! is_inited) {
		fprintf(stderr, "GeneticMain_Main:GeneticMain SubProgram NOT initialized\n");
		return 1;
	}
	
	Screen_MainStart(&sc);
	
	return 0; // important
}

void GeneticMain_Destroy() {
	if (! is_inited) return;
	
	SDL_LockMutex(runMutex);
	if (run) {
		run = false;
		SDL_UnlockMutex(runMutex);
		SDL_WaitThread(thread, NULL);
	}
	else
		SDL_UnlockMutex(runMutex);
	SDL_DestroyMutex(runMutex);
	
	Screen_destroy(&sc);
	
	if (stacklistY) free(stacklistY);
	if (stacklistX) free(stacklistX);
	if (grid) free(grid);
	if (image) {free(image); image = NULL;}
	if (text_block) {free(text_block); text_block=NULL;}
	if (label) {free(label);  label=NULL;}
	if (button) {free(button); button=NULL;}
	if (labelimage) {free(labelimage); labelimage=NULL;}
	if (buttonimage) {free(buttonimage); buttonimage=NULL;}

	is_inited = false;
}
