/*
 * Scenery.h
 *
 *  Created on: 02/10/2016
 *      Author: fpirra
 */

#ifndef SRC_MODEL_GAME_SCENERY_H_
#define SRC_MODEL_GAME_SCENERY_H_

#include "Player.h"
#include "Background.h"
#include <vector>
#include <string>
#include "../../utils/Protocol.h"
#include <queue>
#include "../Event.h"

using namespace std;

class Scenery {
private:
	vector<Player> players;
	vector<Background> backgrounds;
	unsigned int windowWidth, windowHeight;

public:
	Scenery(unsigned int width, unsigned int height);
	virtual ~Scenery();

	void process_key(EventCode keycode, Entity entity);

	vector<struct event> process_keys_queue(queue<Event> keys);

	vector<struct event> obtenerEstadoEscenario();

	// *-* El que llame a player debe chequear si ya se llego al maximo de jugadores permitidos en el nivel
	void addElementToScenery(Player player);

	// El background que se agrega ya debe tener su Z-index definido, asi solamente se agrega al vector
	void addElementToScenery(Background background);

};

#endif /* SRC_MODEL_GAME_SCENERY_H_ */
