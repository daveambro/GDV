#include <Headers.h>
#include <Players.h>

int main(){
	sf::RenderWindow window(sf::VideoMode(600, 400), "SFML works!", sf::Style::Default);
	MPlayer* master=new MPlayer();
	master->todelete = NULL;
	master->win = &window;
	master->start(0);
	window.setKeyRepeatEnabled(false);
	//master->insertkey("enter", true);
	//if (master->keylist["enter"])printf("Reacheditininsert");
	while (window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				window.close();
			}
			if (event.type == sf::Event::KeyPressed){
				if (event.key.code == sf::Keyboard::Return){
					master->insertkey("enter", true);
				}
				if (event.key.code == sf::Keyboard::Down){
					master->insertkey("down", true);
				}

				if (event.key.code == sf::Keyboard::Up){
					master->insertkey("up", true);
				}

				if (event.key.code == sf::Keyboard::Left){
					master->insertkey("left", true);
				}

				if (event.key.code == sf::Keyboard::Right){
					master->insertkey("right", true);
				}

				if (event.key.code == sf::Keyboard::B){
					master->insertkey("recstart", true);
				}

				if (event.key.code == sf::Keyboard::N){
					master->insertkey("recend", true);
				}

				if (event.key.code == sf::Keyboard::M){
					master->insertkey("sensend", true);
				}

				if (event.key.code == sf::Keyboard::O){
					master->insertkey("polpoint", true);
				}

				if (event.key.code == sf::Keyboard::P){
					master->insertkey("polend", true);
				}

				if (event.key.code == sf::Keyboard::I){
					master->insertkey("setstart", true);
				}

				if (event.key.code == sf::Keyboard::K){
					master->insertkey("scaledown", true);
				}

				if (event.key.code == sf::Keyboard::L){
					master->insertkey("scaleup", true);
				}

				

			}

			if (event.type == sf::Event::KeyReleased){
				if (event.key.code == sf::Keyboard::Down){
					master->insertkey("down", false);
				}

				if (event.key.code == sf::Keyboard::Up){
					master->insertkey("up", false);
				}

				if (event.key.code == sf::Keyboard::Left){
					master->insertkey("left", false);
				}

				if (event.key.code == sf::Keyboard::Right){
					master->insertkey("right", false);
				}

				if (event.key.code == sf::Keyboard::Return){
					master->insertkey("enter", false);
				}

				if (event.key.code == sf::Keyboard::B){
					master->insertkey("recstart", false);
				}

				if (event.key.code == sf::Keyboard::N){
					master->insertkey("recend", false);
				}

				if (event.key.code == sf::Keyboard::M){
					master->insertkey("sensend", false);
				}

				if (event.key.code == sf::Keyboard::O){
					master->insertkey("polpoint", false);
				}

				if (event.key.code == sf::Keyboard::P){
					master->insertkey("polend", false);
				}

				if (event.key.code == sf::Keyboard::I){
					master->insertkey("setstart", false);
				}

				if (event.key.code == sf::Keyboard::K){
					master->insertkey("scaledown", false);
				}

				if (event.key.code == sf::Keyboard::L){
					master->insertkey("scaleup", false);
				}
			}

			/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
				master->insertkey("up", true);
			}
			else{ master->insertkey("up", false); }*/
		}
		
		master->update();
		master->draw();
		if (master->todelete != NULL){ delete master->todelete; master->todelete = NULL; }
		
	}
	/*char a;
	scanf_s("%c", &a);*/
	return 0;
}