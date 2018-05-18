/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 02:47:18 by ldedier           #+#    #+#             */
/*   Updated: 2018/05/18 10:28:01 by lcavalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	any_key_pressed(t_world *world)
{
	int i;

	i = -1;
	printf("any_key_pressed called\n");fflush(stdout);
	while (++i < nkeys)
		if (world->keys[i])
			return (1);
	return (0);
}

int		get_input(t_world *e)
{
	SDL_Event event;

	printf("loop called\n");fflush(stdout);
	while (SDL_PollEvent(&event))
	{
		printf("reading key\n");fflush(stdout);
		if (event.type == SDL_KEYDOWN)
			ft_keys_event(e, event, 1);
		if (event.type == SDL_KEYUP)
			ft_keys_event(e, event, 0);
		if (event.type == SDL_MOUSEMOTION)
			ft_mouse_motion(e, event);
		if (event.window.event == SDL_WINDOWEVENT_CLOSE ||
				(event.key.keysym.sym == SDLK_ESCAPE
				 && event.type == SDL_KEYDOWN))
		{
			e->cancel_render = 1;
			printf("cancel AND EXIT OR NOTHING\n");
			join_threads(e);
			return (1);
		}
		else
		printf("no input\n");fflush(stdout);
	}
	if (any_key_pressed(e))
	{
		printf("world %p\n",e);fflush(stdout);
		printf("key pressed. cancel render: %i\n",e->cancel_render);fflush(stdout);
		//if (e->cancel_render == 0)
		//{
		e->keys[mouse_move] = 0;
		//join_threads(e);
		if (e->keys[key_enter] == 1)
		{
			e->keys[key_enter] = 0;
			e->cancel_render = 0;
			printf("render image\n");fflush(stdout);
			paint_threaded(e);
			printf("finished image render\n");fflush(stdout);
			//paint_not_threaded(e);
		}
		else
		{
			ft_process(e);
			e->cancel_render = 1;
			e->progress = 0;
			printf("INPUT PRESSED, CANCELING RENDERING\n");
			paint_threaded_fast(e);
		}
		//}
		//SDL_Delay(12);
	}
	printf("loop finish\n");fflush(stdout);
	return (0);
}

void	ft_loop(t_world *e)
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	paint_threaded_fast(e);
	while (!get_input(e))
		;
	end(e);
}
