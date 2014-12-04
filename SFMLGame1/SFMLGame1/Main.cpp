#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC

#include <wtypes.h>
#include <windef.h>
#include <SFML/Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>
#include <stdio.h>
#include <Box2D\Box2D.h>
#include <sfeMovie\Movie.hpp>
#include <sfeMovie\Visibility.hpp>
#include <io.h>
#include <iostream>
#include <stdlib.h>
#include <boost\archive\binary_iarchive.hpp>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\serialization\list.hpp>
#include <boost\serialization\base_object.hpp>
#include <boost\serialization\utility.hpp>
#include <boost\serialization\assume_abstract.hpp>
#include <fstream>
#include <WinBase.h>
#include <list>
#include <thread>
#include <string>
//#include <Headers.h>
//#include <algorithm>
//#include <math.h>
//music, convex shapes, serialize, blend in level, colorballs, teleport, create passages,moving plattforms,red dangerzones

float easing(float t,float b, float c, float d){
	t = t / d;
	float ts= (t)*t;
	float tc = ts*t;
	return b + c*(-2 * tc + 3 * ts);
};

class verts {
public:float posx, posy;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & posx; ar & posy; printf("and pos");
	}
};

class polys {
public: std::list<verts> poses;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & poses; printf("and pos");
	}
};

class rects {
public: float startx, starty, endx, endy, posx, posy;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & startx; ar & starty; ar & posx; ar & posy; printf("and pos");
	
	}

};
class testlev{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		printf("serializing prims");
		ar & yeah; ar & blah; ar & justtest; ar & polylist; printf("and complist");
		ar & ints; printf("and list");
	}
public:	
	int yeah;
	float blah;
	std::list<rects> justtest;
	std::list<polys> polylist;
	std::list<int> ints;
	b2World* world;
	float32 tstep;
	int32 vIt;
	int32 pIt;
	sf::RenderWindow* win;
	sf::RectangleShape* sth;
	void bla(){ /*printf("shit"); std::cout << "shit";*/ win->draw(*sth); sf::Context blp; blp.setActive(true); };
	void run(){
		while (true){
			//Sleep(1);
			world->Step(tstep, vIt, pIt); printf("simulating");
			
		}
	};

};

void forthread(testlev* test){
	printf("Thread");
	//sf::Context con; con.setActive(true);
	/*sf::Music musi;
	musi.openFromFile("./Music/battle5.ogg");
	musi.play();*/
	test->run();
};

int bla()
{
	sf::ContextSettings sets; sets.antialiasingLevel = 2;
	sf::RenderWindow window(sf::VideoMode(600, 400), "SFML works!", sf::Style::Default, sets);
#if 0
	float t = 0;
	float b = 5;
	float c = 595;
	float d = 3;
	while(true){
		Sleep(10);
		float resx;
		float resy;
		
			resx = easing(t, b, c, d);
			resy = easing(t, 5, 395, d);
		//res = c*(t / d) + b;
			printf("\n %f %f",resx,t);
			t=t+0.01f;
			window.setSize(sf::Vector2u(static_cast<uint16>(resx), static_cast<uint16>(resy)));
		if(resx>=598) break;
	}
#endif
#if 0
	sfe::Movie movie;
	movie.openFromFile("Seq01.mp4");
	movie.fit(0,0,600,400);
	movie.play();
	sf::Music musi;
	musi.openFromFile("./Music/M27.ogg");
	musi.play();
	while(true){
		movie.update();
		window.clear();
		window.draw(movie);
		window.display();
		if (movie.getStatus() != sfe::Status::Playing){ printf("Movieend."); /*delete &movie;*/ break; }
	}
#endif
#if 1	
	sf::CircleShape shape(10.f);
	shape.setFillColor(sf::Color::Green); shape.setOrigin(10.f, 10.f);
	//std::ofstream ofs("sertest.dat",std::ios::binary);
	//boost::archive::binary_oarchive oa(ofs);
	testlev tes;
	std::map<int, std::string> fortests;
	fortests.insert(std::pair<int,std::string>(1,"whatever"));
	std::cout << fortests[1];
	fortests.insert(std::pair<int, std::string>(1, "blabla")).first->second = "blabla"; fortests.find(1)->second = "fuckit";
	std::cout << fortests[1];

	std::map<std::string, bool> keytests;
	keytests.insert(std::pair<std::string, bool>("test", true)).first->second = false;
	if (!keytests["test"])
	std::cout << "works";
	//rects testi; testi.posx = 1;
	//tes.blah = 0.1f; tes.yeah = 1; tes.ints.push_back(1); tes.justtest.push_back(testi);
	//oa << tes;
	//ofs.close();
	b2Vec2 gravity(0.0f, 1.0f);
	b2World world(gravity);
	b2BodyDef gbd;
	gbd.position.Set(0.0f, 200.0f);
	b2Body* gb = world.CreateBody(&gbd);
	b2PolygonShape gbox;
	gbox.SetAsBox(5000.0f, 100.0f);
	gb->CreateFixture(&gbox, 0.0f);

	b2BodyDef bodyd;
	bodyd.type = b2_dynamicBody;
	bodyd.position.Set(0.0f, 100.0f);
	b2Body* mbody = world.CreateBody(&bodyd);


	b2CircleShape circle;
	circle.m_radius = 10.0f;

	b2FixtureDef fixdef;
	fixdef.shape = &circle;
	fixdef.density = 0.5f;
	fixdef.friction = 0.2f;

	mbody->CreateFixture(&fixdef);

	b2BodyDef blockd;
	blockd.type = b2_dynamicBody; blockd.allowSleep = false; blockd.angle = 0; blockd.fixedRotation = false;
	blockd.position.Set(-10, -10); blockd.angularDamping = 0.02f;
	b2Body* block = world.CreateBody(&blockd); 
	b2FixtureDef blockfix;
	b2PolygonShape blocks;
	blocks.SetAsBox(10, 10);
	blockfix.shape = &blocks;
	blockfix.density = 1;
	blockfix.friction = 0.9f;
	blockfix.restitution = 0.01f;
	block->CreateFixture(&blockfix);
	block->ResetMassData();
	float32 tstep = 1.0f / 360.0f;
	int32 vIt = 6;
	int32 pIt = 2;
	b2Vec2* bla = new b2Vec2(100.0f, -500.0f);
	b2Vec2* blu = new b2Vec2;
	sf::View view=window.getView();
	sf::RectangleShape sth(sf::Vector2f(20, 20));
	sth.setPosition(-10, -10); sth.setOrigin(10, 10);
	bool slideall = true;

	sf::Vector2f start;
	sf::Vector2f end;
	sf::RectangleShape newblock;
	std::list<sf::RectangleShape> reclist;
	std::list<b2Body*> bolist;
	std::list<sf::Vector2f>* convlist=new std::list<sf::Vector2f>();
	std::list<sf::ConvexShape> convshlist;

	/*sfe::Movie movie;
	movie.openFromFile("Wildlife.wmv");
	movie.fit(0, 0, 600, 400);
	movie.play();
	sf::RenderTexture rendtex;
	rendtex.create(600, 400);
	movie.update();
	rendtex.draw(movie);*/

	sf::Texture backtex;
	backtex.loadFromFile("vs14.jpg"); backtex.setSmooth(true);
	sf::Sprite backg;
	backg.setTexture(backtex);
	backg.setPosition(0.0f, 0.0f);
	sf::Texture maintex;
	maintex.loadFromFile("main.png");
	sth.setTexture(&maintex);
	sf::Texture clock1tex; clock1tex.setSmooth(true);
	clock1tex.loadFromFile("clock1.png");
	sf::CircleShape clock1; clock1.setTexture(&clock1tex); clock1.setRadius(50);
	sf::Texture clock2tex; clock2tex.setSmooth(true);
	clock2tex.loadFromFile("clock2.png");
	sf::CircleShape clock2; clock2.setTexture(&clock2tex);
	clock2.setRadius(50); clock2.setOrigin(sf::Vector2f(50, 50));
	//clock2.setOrigin(sf::Vector2f((float)clock2tex.getSize().x/2u,(float)clock2tex.getSize().y/2u));
	
	/*sf::Music musi;
	musi.openFromFile("./Music/battle5.ogg");
	musi.play();*/
	testlev* test=new testlev();
	test->world = &world;
	test->tstep = tstep;
	test->vIt = vIt;
	test->pIt = pIt;
	test->win = &window;
	test->sth = &sth;
	sf::Thread* t2=new sf::Thread(&forthread, test);
	t2->launch();
	//std::thread* t1 = new std::thread(&forthread,test);
	
	b2Body* sens=NULL;
	b2Body* bobo=NULL;
	testlev tas;
	while (window.isOpen())
	{
		blu->x = shape.getPosition().x;
		blu->y = shape.getPosition().y;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (sens != NULL){
				if (sens->GetContactList() != NULL){
					if (sens->GetContactList()->contact->IsTouching()){
						printf("Touchdown."); window.close();
					}
				}
			}
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::W == event.key.code){
					printf("Madeit");
					mbody->ApplyLinearImpulse(*bla, *blu, true);
				}

				if (sf::Keyboard::E == event.key.code){
					printf("Madeit");
					mbody->ApplyLinearImpulse(b2Vec2(-100,-500), *blu, true);
				}

				if (sf::Keyboard::Right == event.key.code){
					view.move(5.0f, 0.0f);
					printf("/n %f %f %f %f %f", sth.getRotation(), block->GetAngle(), block->GetTransform().q.GetAngle(), block->GetTransform().q.c, block->GetTransform().q.s);
				}

				if (sf::Keyboard::B == event.key.code){
					/*block->SetTransform(block->GetPosition(), 0.7853);*/ block->SetAwake(true); /*block->SetAngularVelocity(-2);*/
					block->ApplyAngularImpulse(-2000, true); /*block->ApplyTorque(300000, true);*/
					block->ApplyForce(b2Vec2(-100, -50), b2Vec2(block->GetPosition().x, block->GetPosition().y),true);
					printf("/n%f %f", sth.getRotation(), block->GetAngle(), block->GetTransform().q.GetAngle());
				}

				if (sf::Keyboard::J == event.key.code){
					slideall = true;
					/*block->GetFixtureList()->SetFriction(0.9f);*/
					/*block->ApplyAngularImpulse(-2000, true);*//*block->ApplyForceToCenter(b2Vec2(-20, 0), true);*/
					block->SetAngularVelocity(-1); block->SetLinearVelocity(b2Vec2(-4, 0));
				}

				if (sf::Keyboard::L == event.key.code){
					slideall = true;
					/*block->GetFixtureList()->SetFriction(0.9f);*/
					block->SetAngularVelocity(1); /*block->ApplyForceToCenter(b2Vec2(20, 0),true);*/block->SetLinearVelocity(b2Vec2(4, 0));
					//block->ApplyForce(b2Vec2(30000, 0), b2Vec2(block->GetPosition().x, block->GetPosition().y), true);
				}
				//if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)){
				//	block->SetLinearVelocity(b2Vec2(4, 0)); block->SetAngularVelocity(1);
				//}

				if (sf::Keyboard::I == event.key.code){
					if (block->GetLinearVelocity().y <= 5 && block->GetLinearVelocity().y >= -5){
						printf("Jump.");
						block->ApplyLinearImpulse(b2Vec2(block->GetLinearVelocity().x, -1000), b2Vec2(block->GetPosition().x, block->GetPosition().y), true);
					}
				}

				if (sf::Keyboard::K == event.key.code){
					if (slideall){
						/*block->GetFixtureList()->SetFriction(0.3f);*/
						block->SetAngularVelocity(0); block->SetTransform(block->GetPosition(), 0);
						block->ApplyLinearImpulse(b2Vec2(block->GetLinearVelocity().x * 500, 100), b2Vec2(block->GetPosition().x, block->GetPosition().y), true);
						slideall = false;
					}
				}
				if (sf::Keyboard::G == event.key.code){
					world.SetGravity(b2Vec2(1.0f, 0.5f));
				}
				if (sf::Keyboard::N == event.key.code){
					start = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				}
				if (sf::Keyboard::M == event.key.code){
					end = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					sf::Vector2f tmp;
					if (start.x > end.x){ tmp.x = end.x; end.x = start.x; start.x = tmp.x; }
					if (start.y > end.y){ tmp.y = end.y; end.y = start.y; start.y = tmp.y; }
					newblock.setSize(end - start);
					newblock.setPosition(start);
					//newblock.setOrigin((end-start)/2.f);
					reclist.push_back(newblock);
					rects* bltest=new rects();
					bltest->posx = newblock.getPosition().x;
					bltest->posy = newblock.getPosition().y;
					bltest->startx = newblock.getSize().x;
					bltest->starty = newblock.getSize().y;
					tes.justtest.push_back(*bltest);
					printf("%d", tes.justtest.size());
					}
				if (sf::Keyboard::Return == event.key.code){
					for each(sf::RectangleShape var in reclist){
						b2BodyDef* bodef = new b2BodyDef; bodef->type = b2_staticBody;
						bodef->position.Set(var.getPosition().x + var.getSize().x / 2, var.getPosition().y + var.getSize().y / 2);
						bobo = world.CreateBody(bodef);
						b2PolygonShape* bosh = new b2PolygonShape;
						bosh->SetAsBox(var.getSize().x/2,var.getSize().y/2);
						bobo->CreateFixture(bosh, 1.0f); 
						printf("%f %f %f %f", var.getPosition().x, var.getPosition().y, bobo->GetPosition().x, bobo->GetPosition().y);
						printf("%f %f", var.getSize().x, var.getSize().y);
						//bobo->SetTransform(b2Vec2(var.getPosition().x+var.getSize().x/2 , var.getPosition().y+var.getSize().y/2), 0.0f);
						bobo->SetUserData(&var);
						bolist.push_back(bobo);
						}
					}
				if (sf::Keyboard::R == event.key.code){
					shape.setRotation(shape.getRotation() + 45.f);
					}
				if (sf::Keyboard::Z == event.key.code){
					bobo->GetFixtureList()->SetSensor(true);
					sens = bobo;
					}
				if (sf::Keyboard::C == event.key.code){
					convlist->push_back(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
					}
				if (sf::Keyboard::V == event.key.code){
					sf::ConvexShape conshape;
					conshape.setPointCount(convlist->size());
					b2Vec2* vertx = new b2Vec2[convlist->size()];
					int i = 0;
					polys* newpoly = new polys();
					for each(sf::Vector2f var in *convlist){
						conshape.setPoint(i, var);
						vertx[i].Set(var.x, var.y);
						verts* newvert = new verts();
						newvert->posx = var.x;
						newvert->posy = var.y;
						newpoly->poses.push_back(*newvert);
						i++;
					}
					tes.polylist.push_back(*newpoly);
					b2BodyDef* podef = new b2BodyDef; podef->type = b2_staticBody;
					b2Body* pobo = world.CreateBody(podef);
					b2PolygonShape poly;
					poly.Set(vertx, convlist->size());
					pobo->CreateFixture(&poly, 0.0f);
					delete[] vertx;
					convshlist.push_back(conshape);
					convlist->clear();
					
					
					}

				if (sf::Keyboard::Y == event.key.code){
					backg.scale(0.9f, 0.9f);
					}

				if (sf::Keyboard::X == event.key.code){
					backg.scale(1.1f, 1.1f);
					}

				if (sf::Keyboard::Q == event.key.code){
					std::ofstream ofs("sertest.dat", std::ios::binary);
					boost::archive::binary_oarchive oa(ofs);
					oa << tes;
					ofs.close();
				}

				if (sf::Keyboard::O == event.key.code){
					std::ifstream ifs("sertest.dat", std::ios::binary);
					boost::archive::binary_iarchive ia(ifs);
					ia >> tas;
					ifs.close();
					for each(rects var in tas.justtest){
						newblock.setSize(sf::Vector2f(var.startx,var.starty));
						newblock.setPosition(var.posx,var.posy);
						printf("newbox:%f %f", newblock.getSize().x, newblock.getPosition().x);
						reclist.push_back(newblock);
					}
					for each(polys var in tas.polylist){
						sf::ConvexShape newcon;
						newcon.setPointCount(var.poses.size());
						b2Vec2* vertx = new b2Vec2[var.poses.size()];
						int i = 0;
						for each(verts vvar in var.poses){
							newcon.setPoint(i, sf::Vector2f(vvar.posx,vvar.posy));
							vertx[i].Set(vvar.posx, vvar.posy);
							i++;
						}
						b2BodyDef* podef = new b2BodyDef; podef->type = b2_staticBody;
						b2Body* pobo = world.CreateBody(podef);
						b2PolygonShape poly;
						poly.Set(vertx, var.poses.size());
						pobo->CreateFixture(&poly, 0.0f);
						delete[] vertx;
						convshlist.push_back(newcon);
					}
				}


				}
				
				
			
			if (event.type == sf::Event::MouseWheelMoved){
				printf("Mouse"); 
				
					view.zoom(1.0f + event.mouseWheel.delta*0.01f);
				
			}
			if (event.type == sf::Event::MouseButtonPressed){
				if (event.mouseButton.button == sf::Mouse::Right){
					b2BodyDef* blockd=new b2BodyDef;
					blockd->type = b2_dynamicBody; blockd->allowSleep = false;
					blockd->position.Set(-10, -10); blockd->fixedRotation = false; blockd->angularDamping = 0.2f;
					b2Body* block = world.CreateBody(blockd);
					b2FixtureDef* blockfix = new b2FixtureDef;
					b2PolygonShape blocks;
					blocks.SetAsBox(10, 10);
					blockfix->shape = &blocks;
					blockfix->density = 0.6f;
					blockfix->friction = 0.3f;
					block->CreateFixture(blockfix);
				}
				printf("Yeah.");
				sth.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
				block->SetTransform(b2Vec2(sth.getPosition().x,sth.getPosition().y), 0.0f);
				block->SetAwake(true);
			}

		}
		
		//world.Step(tstep, vIt, pIt);
		shape.setPosition(mbody->GetPosition().x,mbody->GetPosition().y);
		sth.setPosition(block->GetPosition().x, block->GetPosition().y);
		sth.setRotation(block->GetTransform().q.GetAngle()*180.f/3.14156f);
		view.setCenter(sth.getPosition());
		clock1.setPosition(window.mapPixelToCoords(sf::Vector2i(0,0)));
		clock2.setPosition(clock1.getPosition() + clock2.getOrigin());
		clock2.rotate(0.005f);
		if (clock2.getRotation() >= 358.f)window.close();
		window.clear();
		window.setView(view);
		
		window.draw(backg);
		window.draw(shape);
		window.draw(sth);
		//test->bla();
		
		bolist.begin();
		/*for each (b2Body* var in bolist)
		{
			if (var->GetUserData() != NULL){
				sf::RectangleShape* bla = (sf::RectangleShape*)var->GetUserData();
				bla->setPosition(var->GetPosition().x, var->GetPosition().y);
				window.draw(*bla);
			}
		}*/
		for each(sf::RectangleShape var in reclist){
			window.draw(var);
		}
		for each(sf::ConvexShape var in convshlist){
			window.draw(var);
		}
		window.draw(newblock);
		window.draw(clock1);
		window.draw(clock2);
		window.display();
	}t2->terminate();
#endif
	return 0;
};