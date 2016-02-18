/*** SOURCE: SDL_Widgets_array_current ***/
struct Widget {
	Object 				object;
	void				(*click_handler)
							(struct Widget*, struct Screen*);
	void				(*press_handler)
							(struct Widget*, struct Screen*);
	void				(*release_handler)
							(struct Widget*, struct Screen*);
	void				(*mouse_enter_handler)
							(struct Widget*, struct Screen*);
	SDL_Surface 		*surf;
	SDL_Rect 			pos;
	
	/* Some parameters pointers (obsolete) ... */
	struct Widget		*parameter;
	void				*void_parameter;
	void				*void_parameter2;
	
	/* Pointer to Screen specified cparam array */
	void 				**cparam;
	
	void 				**vparam;		/*read-only*/
	uint				vparam_size;	/*read-only*/
	uint				vparam_count;	/*read-only*/
	usint				maxx;
	usint				maxy;			// right-lower corner of widget
	
	uint				id;
	bool				cparam_exist;
	widgettype  		type;			// type of widget
	bool 				draggable;		// if draggable 
	bool				dragging;		// true while dragging
	bool 				visible;		// if visible
	bool 				mevent;			// if widget itself changes ...
	bool 				need_reload;	// whether need redraw
	
	uchar				mouse_state;	// key pressed same as in SDL
	bool				mouse_over;		// if mouse pointer over widget
	bool 				mouse_move_listen;
	
	bool				internal;		/** read-only **/
};

struct Screen {
	Object				object;
	SDL_Event			event;
	ScreenBackground	background;
	
	/* Array with widgets on screen */
	struct Widget		**widget;
	struct Widget		*widget_ontop;
	uint				c_widget;
	uint 				size_widget;
	
	SDL_Surface			*screen;
	SDL_Event			*pevent;
	void				(*key_up)(struct Screen*, SDLKey);
	void				(*toogle_drag_on)(struct Screen*);
	void				(*user_event)(struct Screen*, SDL_UserEvent*);
	
	/* Callback array with cparam-s */
	struct Callback		*callback;		/*read-only*/
	
	/* Event info */
	bool				has_exited;
	bool				event_handled;
	bool				pool_events;
	bool				drag_on;
	bool				need_reload;
};			// void mouse_click(Widget *sender, Screen *screen)
			// to prevent passing event through tree of widgets
			// simply set screen->event_handled=true at the beginning
			// of callback function
										
void Screen_MainStart(Screen *screen) {
	Widget *widget; uint i;
	
	screen->need_reload = true;
	screen->has_exited  = false;
	
	while (! screen->has_exited) {
		/* Draw all widgets if needed */
		if (screen->need_reload) Screen_draw(screen);
		
		/* Process events by screen and run user specified callbacks */
		event_loop:
			screen->event_handled = false;
			if (screen->pool_events) SDL_PollEvent(screen->pevent);
			else SDL_WaitEvent(screen->pevent);
			switch (screen->event.type) {
				case SDL_USEREVENT:
					if (screen->user_event)
						screen->user_event(screen, 
							&(screen->event.user));
					else
						fprintf(stderr, "Screen_MainStart: Unhandled\
user event of code %d\n", screen->event.user.code);
					break;
				case SDL_QUIT:	// window close
					screen->has_exited = true; return;
				case SDL_KEYUP:
					if (screen->event.key.keysym.sym == SDLK_F1) {
						screen->drag_on = ! screen->drag_on;
						if (screen->toogle_drag_on) // on Screen level
							screen->toogle_drag_on(screen);
					}
					else {
						if (screen->event.key.keysym.sym == SDLK_ESCAPE) 
						{
							screen->has_exited = true;
							return;
						}
					}
					if (screen->key_up) // on Screen level
						screen->key_up(screen, 
							screen->event.key.keysym.sym); 
					break;
				default:
					if ((screen->widget_ontop) && 
							(screen->widget_ontop->visible)) 
					{ // pass event to widget on top
						if (screen->widget_ontop->type==CONTAINER_TYPE)
							Container_mevent(
							 (Container*)screen->widget_ontop, screen);
						else
							Widget_mevent(screen->widget_ontop, screen);
						if (screen->event_handled) break;
					}
					
					for (i = 0; i < screen->c_widget; i++) {
						widget = screen->widget[i];
						if (widget) {
							if (widget->type == CONTAINER_TYPE)
								Container_mevent((Container*)widget, 
									screen);
							else
								Widget_mevent(widget, screen);
							if (screen->event_handled) break;
						}
					}
					break;
			}	// end switch
			if (screen->has_exited) return;
			if (! screen->need_reload) goto event_loop;
		/* End event_loop */
	} /* End main loop */
}

void Widget_mousePressed(Widget *widget, SDL_Event *event) {
	if (event->button.button == 1) { 	// left button
		if ((widget->draggable) && (widget->visible)) {
			widget->dragging = true;	// start dragging
			stx = event->button.x;
			sty = event->button.y;
			fprintf(stderr, "\n--------------\nStart drag: %s\n", 
				Widget_toString(widget));
		}
	}
}

bool Widget_mouseDragging(Widget *widget, SDL_Event *event) {
	if (widget->dragging) {
		dx = event->motion.x - stx;
		dy = event->motion.y - sty;
		
		if ((widget->pos.x + dx < 0) || (widget->pos.y + dy < 0)) 
			return false;
		
		stx += dx;
		sty += dy;
		
		if (widget->type == CONTAINER_TYPE) 
			Container_dragItems((Container*)widget, dx, dy);
		else {
			widget->pos.x 	+= dx; 
			widget->pos.y 	+= dy; 
			widget->maxx 	+= dx;
			widget->maxy 	+= dy;
		}
		
		return true;
	}
	else return false;
}

void Widget_mouseReleased(Widget *widget, SDL_Event *event) {
	if (widget->dragging) { 
		widget->dragging = false; 
		fprintf(stderr, "End drag:   %s\n", Widget_toString(widget));
	}
}

static bool s;
void mouseEvent(Widget *widget, Screen *screen, uchar _event_type) {
	/* process widget dragging */
	if ((screen->drag_on) && 
		(widget->draggable) && 
		(_event_type == MOUSE_PRESS)) {
		Widget_mousePressed(widget, screen->pevent);
		while (widget->dragging) {
			while (SDL_PollEvent(screen->pevent)) {
				s = Widget_mouseDragging(widget, screen->pevent);
				if (screen->pevent->type == SDL_MOUSEBUTTONUP) {
					Screen_draw(screen);
					widget->dragging = false; 
					fprintf(stderr, "End drag:   %s\n", 
						Widget_toString(widget));
					widget->mouse_state ^= widget->mouse_state;
					if (widget->click_handler) {
						screen->event_handled = true; // execute user f.
						widget->click_handler(widget, screen);
					}
					if (widget->release_handler) {
						screen->event_handled = true; // execute user f.
						widget->release_handler(widget, screen);
					}
					return;
				}
			}
			if (s) Screen_draw(screen);
		}
	}
	
	if (! widget->mevent) return;
	
	switch (widget->type) {		// type of inheriting class is written 
								// in parent class field
		case BUTTON_TYPE:		// runs actions specified 
								// in widget classes
			switch (_event_type) {
				case MOUSE_CLICK:
					Button_mouseClicked((Button*)widget, screen); break;
				case MOUSE_PRESS:
					Button_mousePressed((Button*)widget, screen); break;
				case MOUSE_RELEASE:
					Button_mouseReleased((Button*)widget,screen); break;
				case MOUSE_ENTER:
					Button_mouseEnter((Button*)widget, screen); break;
				case MOUSE_EXIT:
					Button_mouseExit((Button*)widget, screen); break;
				case MOUSE_MOVE:
					Button_mouseMove((Button*)widget, screen); break;
				case MOUSE_DRAG:
					Button_mouseDragging((Button*)widget,screen); break;
				default: return;
			}
			break;
		case BUTTONIMAGE_TYPE:
			/** ... **/
			break;
		case WAVEDRAWBOX_TYPE:
			if (_event_type == MOUSE_PRESS)
				WaveDrawBox_mousePressed(
					((WaveDrawBox*)widget), screen);
			else return;
			break;
		case HISTSTRETCHGRAPH_TYPE:
			if (_event_type == MOUSE_PRESS)
				HistStretchGraph_mousePressed(
					((HistStretchGraph*)widget), screen);
			else return;
			break;
		default: return;
	}
}

void Widget_mevent(Widget *widget, Screen *screen) {
	if ((
			(! widget->click_handler)&&
			(! widget->press_handler)&&
			(! widget->release_handler)&&
			(! widget->mouse_enter_handler)&&
			(! widget->mevent)&&
			(! widget->draggable)&&
			(!PRINT_MOUSE_ENTERS))
			 
		|| 
			(! widget->visible)) {
		return;
	}
	
	switch (screen->event.type) {
		case SDL_MOUSEMOTION:
			if ( ((widget->draggable)||(widget->mevent)) 
					&& (widget->mouse_state)) { // dragging
				mouseEvent(widget, screen, MOUSE_DRAG);
			}
			else { // not dragging
				if (widget->mouse_move_listen) 
					mouseEvent(widget, screen, MOUSE_MOVE);
				if (! widget->mouse_over) { // mouse wasn't over widget
					if (Widget_contains(widget, 
										screen->event.motion.x, 
										screen->event.motion.y)) 
					{
						if (PRINT_MOUSE_ENTERS) 
							fprintf(stderr, "M_ENTER: %s\n", 
											Widget_toString(widget));
						widget->mouse_over = true;
						
						mouseEvent(widget, screen, MOUSE_ENTER);
						if (widget->mouse_enter_handler) {
							screen->event_handled = true;
							widget->mouse_enter_handler(widget, screen);
						}
					}
				}
				else { // mouse was over widget
					if (! Widget_contains(widget, 
											screen->event.motion.x, 
											screen->event.motion.y)) 
					{
						widget->mouse_over = false;
						mouseEvent(widget, screen, MOUSE_EXIT);
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (Widget_contains(widget, 
								screen->event.button.x, 
								screen->event.button.y)) 
			{
				widget->mouse_state |= 
							SDL_BUTTON(screen->event.button.button);
				
				/*! Pass here PRESS event!*/
				if (widget->press_handler) {
					screen->event_handled = true; // execute user f.
					widget->press_handler(widget, screen);
				}
				else
					if (widget->click_handler) 
						screen->event_handled = true;
					
				mouseEvent(widget, screen, MOUSE_PRESS);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (widget->mouse_state) {
				if (Widget_contains(widget, 
										screen->event.button.x, 
										screen->event.button.y
									) &&
					( (widget->mouse_state)&
							(SDL_BUTTON(screen->event.button.button))
					)
				) 
				{
					mouseEvent(widget, screen, MOUSE_CLICK);
					
					/*! Click event handle by user code !*/
					if (widget->click_handler) {
						if ((widget->cparam_exist) && 
							(! widget->cparam)) {
							widget->cparam_exist = 
								Screen_getCParam(screen, 
												widget->click_handler, 
												&(widget->cparam));
						}
						screen->event_handled = true; // execute user f.
						widget->click_handler(widget, screen);
					}
					else 
						if (widget->press_handler) 
							screen->event_handled = true;
					/**/
				}
				mouseEvent(widget, screen, MOUSE_RELEASE);
				widget->mouse_state ^= widget->mouse_state;
				/*! Handle release user event here !*/
				if (widget->release_handler) {  // execute user f.
					screen->event_handled = true;
					widget->release_handler(widget, screen);
				}
				/**/
			}
			break;
	}
}

void StackList_mevent(StackList *stacklist, Screen *screen) {
	Widget *widget;
	uint i=0;
	for (; i < stacklist->count; i++) { // foreach children of stacklist
		widget = stacklist->items[i].widget;
		if (widget) {
			if (widget->type == CONTAINER_TYPE) 
				Container_mevent((Container*)widget, screen);
			else
				Widget_mevent(widget, screen);
			
			if (screen->event_handled) return;
		}
	}
	
	widget = &(stacklist->container.widget);
	if ((widget->draggable) || (widget->mevent))
		Widget_mevent(widget, screen);
}

/*** SOURCE: version with Array of Widgets ***/
void Screen_MainStart(Screen *screen) {
	Widget *widget; uint i = 0; 
	screen->need_reload = true;
	screen->has_exited = false;
	
	while (! screen->has_exited) {
		/* Draw all widgets if needed */
		if (screen->need_reload) Screen_draw(screen);
		
		/* Process events by screen and run user specified callbacks */
		event_loop:
			screen->mevent_handled = screen->mevent_switch = false;
			if (screen->pool_events) SDL_PollEvent(screen->pevent);
			else SDL_WaitEvent(screen->pevent);
		
			switch (screen->event.type) {
				case SDL_USEREVENT:
					if (screen->user_event)
						screen->user_event
								(screen, &(screen->event.user));
					else
						fprintf(stderr, "Screen_MainStart:\
Unhandled user event of code %d\n", screen->event.user.code);
					break;
				case SDL_QUIT:	// window close
					screen->has_exited = true; return;
				case SDL_KEYUP:
					if (screen->event.key.keysym.sym == SDLK_F1) {
						screen->drag_on = ! screen->drag_on;
						if (screen->toogle_drag_on) // on Screen level
							screen->toogle_drag_on(screen);
					}
					else {
						if (screen->event.key.keysym.sym == SDLK_ESCAPE) 
						{
							screen->has_exited = true;
							return;
						}
					}
					if (screen->key_up) // on Screen level
						screen->key_up(screen, 
								screen->event.key.keysym.sym); 
					break;
				default:
					widget =  // get Widget on top
						Array_getCurrentOrNull(screen->widget_array);
					if (widget) {
						Widget_mevent(widget, screen);
						screen->mevent_switch = false;	// for safety
					}
					/* if widget from top DOES NOT handled mevent */
					if (! screen->mevent_handled) {
						/* pass mevent to other widgets (from last) */
						i=Array_getLastItemIndex(screen->widget_array);
						while (true) {
							widget = Array_getItemAtOrNull(
											screen->widget_array, i);
							if (widget) {
								Widget_mevent(widget, screen);
								if (screen->mevent_handled) break;
							}
							if (! i) break;
							i--;
						}
						/** if [i] widget want to be switched on top
						 * @NOTE: also mevent_handled must be true
						 * @HERE: change only index of current widget */
						if (screen->mevent_switch)
							Array_setCurrentToIndex(
									screen->widget_array, i);
					}
					
					// Perform switching active widget
					if (screen->mevent_switch) {
						Array_moveCurrentToLast(screen->widget_array);
					}
					else { // none widget want to be switched
						// if ((none widget handled mevent) && 
						// (mouse pressed by first button))
						if ((! screen->mevent_handled) && 
							(screen->event.type == SDL_MOUSEBUTTONDOWN) 
							&& (screen->event.button.button < 4)) 
						{   // unset current(active) widget
						  Array_setCurrentToNone(screen->widget_array);
						  screen->need_reload = true;
						}
					}
					
					break;
			}	// end switch
			if (screen->has_exited) return;
			if (! screen->need_reload) goto event_loop; //break;
		//}	// end WaitEvent loop
	} /* End main loop */
}


void Widget_drag(Widget *widget, Screen *screen) {
	usint stx = screen->event.button.x;
	usint sty = screen->event.button.y;
	signed short dx, dy;
	bool drag = false;
	fprintf(stderr, "\n--------------\nStart drag: %s\n", 
		Widget_toString(widget));
	widget->dragging = true;	// start dragging
	while (widget->dragging) {
		while (SDL_PollEvent(screen->pevent)) {
			if (screen->event.type == SDL_MOUSEMOTION) {
				dx = screen->event.motion.x - stx;
				dy = screen->event.motion.y - sty;

				if ((widget->pos.x + dx >= 0) && 
						(widget->pos.y + dy >= 0)) 
				{
					stx += dx;
					sty += dy;
					drag = true;
					if (widget->type == CONTAINER_TYPE) 
						Container_dragItems((Container*)widget, 
											dx, dy);
					else {
						widget->pos.x 	+= dx; 
						widget->pos.y 	+= dy; 
						widget->maxx 	+= dx;
						widget->maxy 	+= dy;
					}
					Screen_draw(screen);
				}
			}
			else {
				if (screen->event.type == SDL_MOUSEBUTTONUP) {
					fprintf(stderr, "End drag:   %s\n", 
						Widget_toString(widget));
					if (drag) {
						widget->mouse_state ^= widget->mouse_state;
						//Screen_draw(screen); 
						// redraw done by setting:
						// screen->need_reload=true in Widget_mevent
						widget->dragging = false;
					}
					return;
				}
			}
		}
	}
}

void Widget_mevent(Widget *widget, Screen *screen) {
	if ((! widget) || (! screen)) return;
	if (! widget->visible) return;
	
	if (widget->type == CONTAINER_TYPE) {
		Container_mevent((Container*)widget, screen);
		if (screen->mevent_handled) {
			if ((! widget->parent) && 
				(screen->event.type == SDL_MOUSEBUTTONDOWN) && 
				(screen->event.button.button < 4))
				{ 
					fprintf(stderr, "Set to switch by container\n"); 
					screen->mevent_switch = screen->need_reload = true; 
				}
			return;
		}
	}
	
	if ((widget->has_user_mevents) || 
		(widget->has_internal_mevents) ||
		(widget->draggable)) 
	{
		uchar type = MOUSE_NONE;
		uchar but;
		bool  	handled  = false, 
				contains = false;
		
		switch (screen->event.type) {
			case SDL_MOUSEMOTION:	// mouse moved
				if (widget->mouse_state) {
					type = MOUSE_DRAG; // at least one button is pressed
				}
				else {                 // all buttons are released
					contains = Widget_contains(widget, 
												screen->event.motion.x, 
												screen->event.motion.y);
					if (widget->mouse_over) 
					{ // mouse pointer WAS over widget
						if (! contains) 
						{ // NOW widget doesn't contains mouse pointer
							widget->mouse_over = false;
							type = MOUSE_EXIT;
						}
					}
					else 
					{ // mouse pointer WASN't over widget
						if (contains) 
						{ // NOW widget is covered by mouse pointer
							widget->mouse_over = true;
							type = MOUSE_ENTER;
						}
					}
					/*! Here may set type |= MOUSE_MOVE 
					 *! (something like that) to listen to 
					 *! mouse move events 
					 *! by now they are unsupported */
				}
				break;
			case SDL_MOUSEBUTTONDOWN:	// mouse button down
				contains = Widget_contains(widget, 
								screen->event.button.x, 
								screen->event.button.y);
				if (contains) 
				{	// if widget contains mouse pointer
					// set exact bit of .mouse_state
					widget->mouse_state |= 
								SDL_BUTTON(screen->event.button.button);
					type = MOUSE_PRESS;
				}
				break;
			case SDL_MOUSEBUTTONUP:		// mouse button up
				contains = Widget_contains(widget, 
												screen->event.button.x, 
												screen->event.button.y);
				if ((widget->mouse_state) &
					(but = SDL_BUTTON(screen->event.button.button))) 
				{ 	// unset exact bit of .mouse_state
					widget->mouse_state &= ~but;
					type = MOUSE_RELEASE;
				}
				break;
		}
		
		if (type != MOUSE_NONE) 
		{	// proceed with internal and user events
			if (widget->has_internal_mevents) {
				switch (type) {
					case MOUSE_PRESS:
						if (widget->internal_mevent.mouse_press) { 
							handled = true; widget->internal_mevent.
										mouse_press(widget, screen); } 
						break;
					case MOUSE_RELEASE:	
						if (widget->internal_mevent.mouse_release) { 
							handled = true; widget->internal_mevent.
										mouse_release(widget, screen);}
						if ((widget->internal_mevent.mouse_click) && 
							(contains)) {
							handled = true; widget->internal_mevent.
										mouse_click(widget, screen); } 
						break;
					case MOUSE_ENTER:
						if (widget->internal_mevent.mouse_enter) { 
							handled = true; widget->internal_mevent.
										mouse_enter(widget, screen); } 
						break;
					case MOUSE_EXIT:
						if (widget->internal_mevent.mouse_exit) { 
							handled = true; widget->internal_mevent.
										mouse_exit(widget, screen); }
						break;
					case MOUSE_DRAG:
						if (widget->internal_mevent.mouse_drag) {
							handled = true; widget->internal_mevent.
										mouse_drag(widget, screen); }
						break;
				};
			}
			if (widget->has_user_mevents) {
				switch (type) {
					case MOUSE_PRESS:
						if (widget->user_mevent.mouse_press) { 
							handled = true; widget->user_mevent.
										mouse_press(widget, screen); }
						break;
					case MOUSE_RELEASE:
						if (widget->user_mevent.mouse_release) { 
							handled = true; widget->user_mevent.
									mouse_release(widget, screen); }
						if ((widget->user_mevent.mouse_click) && 
							(contains)) {
							handled = true; 
							if ((widget->cparam_exist) && 
								(! widget->cparam)) 
							{
								widget->cparam_exist = Screen_getCParam
									(	screen, 
										widget->user_mevent.mouse_click, 
										&(widget->cparam));
							}
							widget->user_mevent.
									mouse_click(widget, screen); }
						break;
					case MOUSE_ENTER:
						if (widget->user_mevent.mouse_enter) {
							handled = true; widget->user_mevent.
										mouse_enter(widget, screen); }
						break;
					case MOUSE_EXIT:
						if (widget->user_mevent.mouse_exit) {
							handled = true; widget->user_mevent.
										mouse_exit(widget, screen); }
						break;
					case MOUSE_DRAG:
						if (widget->user_mevent.mouse_drag) {
							handled = true; widget->user_mevent.
										mouse_drag(widget, screen); }
						break;
				};
			}
			if ((widget->draggable) && 
				(type == MOUSE_PRESS) && 
				(widget->mouse_state == SDL_BUTTON_LMASK) && 
				(screen->drag_on)) 
			{
				handled = true;
				if (screen->need_reload) Screen_draw(screen);
				screen->mevent_handled = true;
				screen->mevent_switch  = true;
				Widget_drag(widget, screen); //! perform dragging
				if (widget->has_internal_mevents) 
				{ //! call release and click internal mevents
					if (widget->internal_mevent.mouse_release) {
						widget->internal_mevent.mouse_release
													(widget, screen); }
					if (widget->internal_mevent.mouse_click) {
						widget->internal_mevent.mouse_click
													(widget, screen); }
				}
				if (widget->dragging) 
				{ //! widget was pressed and released in same point
					if (widget->has_user_mevents) 
					{ //! call release and click user mevents
						if (widget->user_mevent.mouse_release) {
							widget->user_mevent.mouse_release
													(widget, screen); }
						if (widget->user_mevent.mouse_click) {
							if ((widget->cparam_exist) && 
								(! widget->cparam)) {
								widget->cparam_exist = Screen_getCParam
									(screen, 
									widget->user_mevent.mouse_click, 
									&(widget->cparam));
								}
							widget->user_mevent.mouse_click
													(widget, screen);
						}
					}
					widget->dragging = false;
					
				}
				screen->need_reload    = true;
			}
		}
		
		if (	(! widget->parent) && 
				(screen->event.type == SDL_MOUSEBUTTONDOWN) &&
				(screen->event.button.button < 4) && 
				(contains)) 
		{
			screen->mevent_handled = true;
			screen->mevent_switch  = true;
		}
		else {
			if (handled) {
				screen->mevent_handled = true;
				/**
				fprintf(stderr, "[Proceed with event(%s)]\
Setting only mevent_handled: %d, %d, %d, %d\n", 
				Screen_getEventName(screen->event.type),
				(! widget->parent), 
				(screen->event.type == SDL_MOUSEBUTTONDOWN), 
				(screen->event.button.button < 4), 
				(contains)); **/
			}
		}
		if (contains) screen->mevent_handled = true;
		/** fprintf(stderr, "\t### Was event %d\n", 
		  screen->mevent_handled); **/
	}
	else 
	{ // none event made but maybe need to stop event 
	  // from passing to other widgets
	   /**
		if ((! widget->parent) 	&&
			(
				((screen->event.type == SDL_MOUSEMOTION) && 
				(Widget_contains(widget, 
									screen->event.motion.x, 
									screen->event.motion.y))) ||
				((screen->event.type == SDL_MOUSEBUTTONDOWN) && 
					(Widget_contains(widget, 
										screen->event.button.x, 
										screen->event.button.y)))
			)
					
			screen->mevent_handled = true;
		}
		fprintf(stderr, "\t### NO event\n"); **/
	}
}

void StackList_mevent(StackList *stacklist, Screen *screen) {
	Widget *widget; uint i=0;
	
	for (; i < stacklist->count; i++) {
		widget = stacklist->items[i].widget;
		if (widget) {
			Widget_mevent(widget, screen);
			
			if (screen->mevent_handled) return;
		}
	}
}
