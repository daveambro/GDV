#pragma once
#include <Headers.h>
#include <Players.h>

class verts {
public:float posx;
	   float posy;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & posx; ar & posy;
	}
};


class polys {
public: std::list<verts> poses;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & poses;
	}
};

class rects {
public: float startx;
		float starty;
		float posx;
		float posy;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & startx; ar & starty; ar & posx; ar & posy;
	}

};

class leveltemp{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & rectlist; ar & polylist; ar & playerpos; ar & backscale; ar & endrec; ar & plpo;
	}
public:
	std::list<rects> rectlist;
	std::list<polys> polylist;
	std::list<verts> plpo;
	verts playerpos;
	verts backscale;
	rects endrec;
	
};

class threadsim {
public:
	b2World* world;
	float32 tstep;
	int32 vIt;
	int32 pIt;
	void run(){
		sf::Clock clock;
		while (true){
			
			world->Step(clock.restart().asMicroseconds()*tstep, vIt, pIt);
		}
	}
};

void forthread(threadsim* test){
	test->run();
};

class Masterplayer{
public:
	   std::map<std::string, bool> keylist;
	  // std::map<int, Playerobject*> playlist;
	   int cur;
	   //Playerobject* player;
	   sf::RenderWindow* win;
	   sf::Music music;

	  virtual void change(int i){
		   //delete player;
		   //start(i);
	   }
	   void insertkey(std::string key,bool val){
		   keylist.insert(std::pair<std::string, bool>(key, val)).first->second = val;
	   }
	  virtual void start(int i){
		   /*if (i == 0){
			   player = new Intromovie();
			   player->moviefile = "Wildlife.wmv";
		   }
		   player->init();*/
	   }
	   virtual void update(){
		   //player->update();
	   }

	  virtual void draw(){
		  // player->draw();
	   }
};


class Playerobject{
public:
	std::string moviefile;
	sfe::Movie movie;

	sf::RenderWindow* win;
	sf::View* view;

	b2World* world;
	b2Body* playersim;
	sf::RectangleShape* player;
	bool slide;
	b2Body* endsens;

	Masterplayer* master;
	int nextplayer;

	std::string backpic;
	sf::Sprite* background;

	float time;
	sf::CircleShape* clock1;
	sf::CircleShape* clock2;

	sf::Thread* t1;

	std::string levelfile;
	leveltemp* lvls;
	sf::Vector2f recstart;
	std::list<sf::RectangleShape> tmprec;
	std::list<sf::ConvexShape> tmpconv;
	std::list<sf::Vector2f> tmpvec;
	sf::RectangleShape tmpplayer;
	sf::Texture playtex;
	sf::Texture backtex;
	sf::Texture clock1tex;
	sf::Texture clock2tex;


	void createmovie(){
		movie.openFromFile(moviefile);
		movie.fit(0, 0, 600, 400);
		movie.play();
	}
	void updatemovie(){
		movie.update();
		if (movie.getStatus() != sfe::Status::Playing){ change(); }
	}
	void change(){
		master->change(nextplayer);
	}

	void createlevel(){
		printf("trying file");
		leveltemp lvl;
		//std::ifstream ifs(levelfile,std::ios::binary);
		//boost::archive::binary_iarchive ia(ifs);
		//ia >> lvl; printf("madeit");
		//ifs.close();
		std::string line;
		std::ifstream tfs("textfile.txt");
		while (std::getline(tfs, line)){
			if (line == "playerpos"){
				getline(tfs, line);
				lvl.playerpos.posx = ::atof(line.c_str());
				getline(tfs, line);
				lvl.playerpos.posy = ::atof(line.c_str());
			}
			if (line == "backscale"){
				getline(tfs, line);
				lvl.backscale.posx = ::atof(line.c_str());
				getline(tfs, line);
				lvl.backscale.posy = ::atof(line.c_str());
			}
			if (line == "endrec"){
				getline(tfs, line);
				lvl.endrec.posx = ::atof(line.c_str());
				getline(tfs, line);
				lvl.endrec.posy = ::atof(line.c_str());
				getline(tfs, line);
				lvl.endrec.startx = ::atof(line.c_str());
				getline(tfs, line);
				lvl.endrec.starty = ::atof(line.c_str());
			}
			if (line == "rects"){
				rects* newrec=new rects();
				getline(tfs, line);
				newrec->posx = ::atof(line.c_str());
				getline(tfs, line);
				newrec->posy = ::atof(line.c_str());
				getline(tfs, line);
				newrec->startx = ::atof(line.c_str());
				getline(tfs, line);
				newrec->starty = ::atof(line.c_str());
				lvl.rectlist.push_back(*newrec);
			}
			if (line == "polys"){
				polys* newpoly = new polys();
				while (getline(tfs, line)){
					if (line == "verts"){
						verts* newv = new verts();
						getline(tfs, line);
						newv->posx = ::atof(line.c_str());
						getline(tfs, line);
						newv->posy = ::atof(line.c_str());
						newpoly->poses.push_back(*newv);
					}
					if (line == "endpolys")break;
				}
				lvl.polylist.push_back(*newpoly);
			}
		}
		tfs.close();
		printf("opened file");
		b2Vec2 gravity(0.0f, 1.0f);
		world = new b2World(gravity);

		for each(polys var in lvl.polylist){
			b2Vec2* vertx = new b2Vec2[var.poses.size()];
			int i = 0;
			for each(verts vvar in var.poses){
				vertx[i].Set(vvar.posx, vvar.posy);
				i++;
			}
			b2BodyDef* podef = new b2BodyDef; podef->type = b2_staticBody;
			b2Body* pobo = world->CreateBody(podef);
			b2PolygonShape* poly=new b2PolygonShape;
			poly->Set(vertx, var.poses.size());
			pobo->CreateFixture(poly, 0.0f);
			delete[] vertx;
		}

		for each(rects var in lvl.rectlist){
			printf("%f %f %f %f", var.posx, var.posy, var.startx, var.starty);
			b2BodyDef* bodef = new b2BodyDef; bodef->type = b2_staticBody;
			bodef->position.Set(var.posx + var.startx / 2, var.posy + var.starty / 2);
			b2Body* bobo = world->CreateBody(bodef);
			b2PolygonShape* bosh = new b2PolygonShape;
			bosh->SetAsBox(var.startx / 2, var.starty / 2);
			bobo->CreateFixture(bosh, 1.0f);
		}

		b2BodyDef* bodef = new b2BodyDef; bodef->type = b2_staticBody;
		bodef->position.Set(lvl.endrec.posx + lvl.endrec.startx / 2, lvl.endrec.posy + lvl.endrec.starty / 2);
		b2Body* bobo = world->CreateBody(bodef);
		b2PolygonShape* bosh = new b2PolygonShape;
		bosh->SetAsBox(lvl.endrec.startx / 2, lvl.endrec.starty / 2);
		bobo->CreateFixture(bosh, 1.0f);
		bobo->GetFixtureList()->SetSensor(true);
		endsens = bobo;



		b2BodyDef* playd=new b2BodyDef;
		playd->type = b2_dynamicBody;
		playd->allowSleep = false;
		playd->position.Set(lvl.playerpos.posx, lvl.playerpos.posy);
		
		printf("playerpos:%f %f",playd->position.x,playd->position.y);
		//int i; scanf_s("%d", &i);
		playd->angularDamping = 0.02f;
		playersim = world->CreateBody(playd);
		b2PolygonShape* playsh=new b2PolygonShape;
		playsh->SetAsBox(10, 10);
		b2FixtureDef* playfix=new b2FixtureDef;
		playfix->shape = playsh;
		playfix->density = 1;
		playfix->friction = 0.9f;
		playfix->restitution = 0.01f;
		playersim->CreateFixture(playfix);
		playersim->ResetMassData();
		player = new sf::RectangleShape(sf::Vector2f(20, 20));
		player->setOrigin(10, 10);
		playtex.loadFromFile("main.png");
		player->setTexture(&playtex);
		slide = true;

		
		backtex.loadFromFile(backpic); backtex.setSmooth(true);
		background = new sf::Sprite(backtex);
		background->setPosition(0.0f, 0.0f);
		background->setScale(lvl.backscale.posx, lvl.backscale.posy);
		printf("%f %f", lvl.backscale.posx, lvl.backscale.posy);
		//char a;
		//scanf_s("%c", &a);

		clock1tex.setSmooth(true);
		clock1tex.loadFromFile("clock1.png");
		clock1 = new sf::CircleShape(); clock1->setTexture(&clock1tex); clock1->setRadius(50);
		clock2tex.setSmooth(true);
		clock2tex.loadFromFile("clock2.png");
		clock2 = new sf::CircleShape(); clock2->setTexture(&clock2tex);
		clock2->setRadius(50); clock2->setOrigin(sf::Vector2f(50, 50));
		view = new sf::View();
		*view = win->getView();

		threadsim* sim = new threadsim();
		sim->world = world;
		sim->tstep = 1.0f / 30000.0f;
		sim->vIt = 6;
		sim->pIt = 2;
		t1 = new sf::Thread(&forthread, sim);
		t1->launch();

	}

	void savelevel(){
		std::ofstream ofs("newlevel.dat",std::ios::binary);
		boost::archive::binary_oarchive oa(ofs);
		oa << lvls;
		ofs.close();
		std::ofstream tfs("textfile.txt");
		tfs << "playerpos\n";
		tfs << lvls->playerpos.posx << "\n";
		tfs << lvls->playerpos.posy << "\n";
		tfs << "backscale\n";
		tfs << lvls->backscale.posx << "\n";
		tfs << lvls->backscale.posy << "\n";
		tfs << "endrec\n";
		tfs << lvls->endrec.posx << "\n";
		tfs << lvls->endrec.posy << "\n";
		tfs << lvls->endrec.startx << "\n";
		tfs << lvls->endrec.starty << "\n";
		for each(rects var in lvls->rectlist){
			tfs << "rects\n";
			tfs <<var.posx << "\n";
			tfs << var.posy << "\n";
			tfs << var.startx << "\n";
			tfs << var.starty << "\n";
		}
		for each(polys var in lvls->polylist){
			tfs << "polys\n";
			for each(verts vvar in var.poses){
				tfs << "verts\n";
				tfs << vvar.posx << "\n";
				tfs << vvar.posy << "\n";
			}
			tfs << "endpolys\n";
		}
		tfs.close();
	}

	void updateeditor(){
		printf("\nReal update");
		if (master->keylist["recstart"]){//b
			master->insertkey("recstart", false);
			recstart = win->mapPixelToCoords(sf::Mouse::getPosition(*win));
		}

		if (master->keylist["recend"]){//N
			master->insertkey("recend", false);
			sf::Vector2f end = win->mapPixelToCoords(sf::Mouse::getPosition(*win));
			sf::Vector2f tmp;
			if (recstart.x > end.x){ tmp.x = end.x; end.x = recstart.x; recstart.x = tmp.x; }
			if (recstart.y > end.y){ tmp.y = end.y; end.y = recstart.y; recstart.y = tmp.y; }
			sf::RectangleShape newblock;
			newblock.setSize(end - recstart);
			newblock.setPosition(recstart);
			//newblock.setOrigin((end-start)/2.f);
			tmprec.push_back(newblock);
			rects* bltest = new rects();
			bltest->posx = newblock.getPosition().x;
			bltest->posy = newblock.getPosition().y;
			bltest->startx = newblock.getSize().x;
			bltest->starty = newblock.getSize().y; printf("%f %f %f %f", bltest->posx, bltest->posy, bltest->startx, bltest->starty);
			//char a; scanf_s("%c", &a);
			lvls->rectlist.push_back(*bltest);
		}

		if (master->keylist["sensend"]){//M
			master->insertkey("sensend", false);
			sf::Vector2f end = win->mapPixelToCoords(sf::Mouse::getPosition(*win));
			sf::Vector2f tmp;
			if (recstart.x > end.x){ tmp.x = end.x; end.x = recstart.x; recstart.x = tmp.x; }
			if (recstart.y > end.y){ tmp.y = end.y; end.y = recstart.y; recstart.y = tmp.y; }
			sf::RectangleShape newblock;
			newblock.setSize(end - recstart);
			newblock.setPosition(recstart);
			//newblock.setOrigin((end-start)/2.f);
			tmprec.push_back(newblock);
			rects* bltest = new rects();
			bltest->posx = newblock.getPosition().x;
			bltest->posy = newblock.getPosition().y;
			bltest->startx = newblock.getSize().x;
			bltest->starty = newblock.getSize().y;
			lvls->endrec = *bltest;
		}

		if (master->keylist["polpoint"]){//o
			master->insertkey("polpoint", false);
			tmpvec.push_back(win->mapPixelToCoords(sf::Mouse::getPosition(*win)));
		}

		if (master->keylist["polend"]){//p
			master->insertkey("polend", false);
			sf::ConvexShape conshape;
			conshape.setPointCount(tmpvec.size());
			int i = 0;
			polys* newpoly = new polys();
			for each(sf::Vector2f var in tmpvec){
				conshape.setPoint(i, var);
				verts* newvert = new verts();
				newvert->posx = var.x;
				newvert->posy = var.y;
				newpoly->poses.push_back(*newvert);
				i++;
			}
			lvls->polylist.push_back(*newpoly);
			tmpconv.push_back(conshape);
			tmpvec.clear();
		}

		if (master->keylist["setstart"]){//i
			master->insertkey("setstart", false);
			tmpplayer.setPosition(win->mapPixelToCoords(sf::Mouse::getPosition(*win)));
			lvls->playerpos.posx = tmpplayer.getPosition().x;
			lvls->playerpos.posy = tmpplayer.getPosition().y;
			verts* plps = new verts();
			plps->posx = tmpplayer.getPosition().x;
			plps->posy = tmpplayer.getPosition().y;
			lvls->plpo.push_front(*plps);
			printf("playerpos:%f %f", lvls->playerpos.posx, lvls->playerpos.posy);
			//int i; scanf_s("%d", &i);
		}

		if (master->keylist["scaledown"]){//k
			background->scale(0.999f, 0.999f);
			lvls->backscale.posx = background->getScale().x;
			lvls->backscale.posy = background->getScale().y;
			printf("%f %f", lvls->backscale.posx, lvls->backscale.posy);
			//char a;
			//scanf_s("%c", &a);
		}

		if (master->keylist["scaleup"]){//L
			background->scale(1.001f, 1.001f);
			lvls->backscale.posx = background->getScale().x;
			lvls->backscale.posy = background->getScale().y;
			printf("%f %f", lvls->backscale.posx, lvls->backscale.posy);
		}

		if (master->keylist["enter"]){
			master->insertkey("enter", false);
			savelevel();
			*view = win->getDefaultView();
			win->setView(*view);
			change();
		}

		if (master->keylist["up"]){
			view->move(0.f, -1.f);
		}

		if (master->keylist["down"]){
			view->move(0.f, 1.f);
		}

		if (master->keylist["left"]){
			view->move(-1.f, 0.f);
		}

		if (master->keylist["right"]){
			view->move(1.f, 0.f);
		}
		win->clear();
		win->setView(*view);
		printf("\n End Real Update");


	}

	void updatelevel(){
		printf("\nReal Levelupdate");
		if (endsens != NULL){
			printf("Im closing");
			//win->close();
			if (endsens->GetContactList() != NULL){
				if (endsens->GetContactList()->contact->IsTouching()){
					t1->terminate(); delete world;
					change();
				}
			}
		}
		printf("Step1");
		if (master->keylist["enter"]){
			master->insertkey("enter", false);
			
			printf("Shit happens");
			t1->terminate();
			nextplayer = 1;
			master->music.stop();
			change();
		}

		if (master->keylist["up"]){
			if (playersim->GetLinearVelocity().y <= 5 && playersim->GetLinearVelocity().y >= -5){
				playersim->ApplyLinearImpulse(b2Vec2(playersim->GetLinearVelocity().x, -1000), b2Vec2(playersim->GetPosition().x, playersim->GetPosition().y), true);
			}
		}

		if (master->keylist["left"]){
			slide = true;
			playersim->SetAngularVelocity(-1); playersim->SetLinearVelocity(b2Vec2(-4, 0));
		}

		if (master->keylist["right"]){
			slide = true;
			playersim->SetAngularVelocity(1); playersim->SetLinearVelocity(b2Vec2(4, 0));
		}

		if (master->keylist["down"]){
			if (slide){
				playersim->SetAngularVelocity(0); playersim->SetTransform(playersim->GetPosition(), 0);
				playersim->ApplyLinearImpulse(b2Vec2(playersim->GetLinearVelocity().x * 600, 100), b2Vec2(playersim->GetPosition().x, playersim->GetPosition().y), true);
				slide = false;
			}
		}
		printf("\nStep2");
		player->setPosition(playersim->GetPosition().x, playersim->GetPosition().y);
		player->setRotation(playersim->GetTransform().q.GetAngle()*180.f / 3.14156f);
		printf("\nStep3");
		view->setCenter(player->getPosition());
		clock1->setPosition(win->mapPixelToCoords(sf::Vector2i(0, 0)));
		clock2->setPosition(clock1->getPosition() + clock2->getOrigin());
		clock2->rotate(0.005f*time);
		if (clock2->getRotation() >= 358.f){ t1->terminate(); nextplayer = 1; change(); }
		win->clear();
		win->setView(*view);
		printf("\nStep4");
	}
	void draweditor(){
		printf("\n Real drawing");
		win->draw(*background);
		for each(sf::RectangleShape var in tmprec){
			win->draw(var);
		}
		for each(sf::ConvexShape var in tmpconv){
			win->draw(var);
		}
		win->draw(tmpplayer);
		win->display();
		printf("\n End Real drawing");
	}
	void drawlevel(){
		win->draw(*background);
		win->draw(*player);
		win->draw(*clock1);
		win->draw(*clock2);
		win->display();
	}
	virtual void init(){}
	virtual void update(){}
	virtual void draw(){}

};



class Intromovie :public Playerobject{
public:
	virtual void init(){
		createmovie();
	}
	virtual void update(){
		if (master->keylist["enter"])win->close();
		updatemovie();
		if (movie.getStatus() != sfe::Status::Playing){ change(); }
	}

	virtual void draw(){
		printf("Reached drawing");
		win->clear();
		win->draw(movie);
		win->display();
	}
};

class Menuob : public Playerobject{
public:
	sf::RectangleShape menu;
	sf::Texture* menuitems;
	int curitem;

	virtual void init(){
		printf("\nreached menuinit");
		menuitems = new sf::Texture[5];
		menuitems[0].loadFromFile("./Menu/menu1.png");
		menuitems[1].loadFromFile("./Menu/menu2.png");
		menuitems[2].loadFromFile("./Menu/menu3.png");
		menuitems[3].loadFromFile("./Menu/menu4.png");
		menuitems[4].loadFromFile("./Menu/menu5.png");
		curitem = 0;
		menu.setSize(sf::Vector2f(600, 400));
		menu.setPosition(0, 0);
		printf("\nfinished menuinit");
	}

	virtual void update(){
		printf("\nReached update");
		if (master->keylist["down"]){ curitem++; master->insertkey("down", false); }
		if (master->keylist["up"]){ curitem--; master->insertkey("up", false); }
		if (master->keylist["enter"]){
			master->insertkey("enter", false);
			if (curitem == 0){
				nextplayer = 3;
				change();
			}
			if (curitem == 1){
				nextplayer = 2;
				change();
			}
			if (curitem == 4){
				win->close();
			}
			
		}
		if (curitem < 0)curitem = 4;
		if (curitem>4)curitem = 0;
		menu.setTexture(&menuitems[curitem]);
		sf::View view = win->getDefaultView();
		win->setView(view);
		printf("\nFinished update");
	}

	virtual void draw(){
		//printf("\nReached drawing");
		win->clear();
		win->draw(menu);
		win->display();
		//printf("\nFinished drawing");
	}
};

class Editorob : public Playerobject{
public:
	sf::Texture texts;
	sf::Texture tmptex;
	virtual void init(){
		printf("\n Editorinitstart");
		lvls = new leveltemp();
		texts.loadFromFile("foreditor.png");
		background = new sf::Sprite(texts);
		background->setPosition(0.0f, 0.0f);
		tmpplayer.setSize(sf::Vector2f(20, 20));
		tmpplayer.setOrigin(10, 10);
		tmpplayer.setPosition(0, 0);
		tmptex.loadFromFile("main.png");
		tmpplayer.setTexture(&tmptex);
		view = new sf::View();
		*view = win->getView();
		printf("\nEditorinitend");
	}
	virtual void update(){
		printf("\nEditorupdatestart");
		updateeditor();
		printf("\nEditorupdateend");
	}
	virtual void draw(){
		printf("\nEditordrawstart");
		draweditor();
		printf("\nEditordrawend");
	}
};

class Levelob : public Playerobject{
public:
	virtual void init(){
		printf("\nReached Levelinit");
		createlevel();
		printf("\nReached EndLevelinit");
	}

	virtual void update(){
		printf("\nReached Levelupdate");
		updatelevel();
		printf("\nReached EndLevelupdate");
	}

	virtual void draw(){
		printf("\nReached Leveldraw");
		drawlevel();
		printf("\nReached EndLeveldraw");
	}
};

class MPlayer : public Masterplayer{
public:
	Playerobject* player;
	Playerobject* todelete;
	virtual void change(int i){
		todelete = player;
		start(i);
	}
	virtual void start(int i){
		if (i == 0){
			cur = i;
			player = new Intromovie();
			player->moviefile = "Seq01.mp4";
			player->nextplayer = 1;
			music.openFromFile("./Music/M27.ogg");
			music.play();
		}
		if (i == 1){
			printf("reached creation of menu");
			cur = i;
			player = new Menuob();
		}
		if (i == 2){
			printf("reached editorcreation");
			cur = i;
			player = new Editorob();
			player->nextplayer = 1;
		}
		if (i == 3){
			cur = i;
			player = new Levelob();
			player->nextplayer = 1;
			player->backpic = "./Level/level1.png";
			player->levelfile = "./Level/level1.dat";
			music.openFromFile("./Music/MGO.ogg");
			player->time = 2;
			music.play();
		}
		player->win = win;
		player->master = this;
		player->init();
		player->update();
	}
	virtual void update(){
		player->update();
	}

	virtual void draw(){
		player->draw();
	}
};