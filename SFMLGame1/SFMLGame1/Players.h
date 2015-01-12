#pragma once
#include <Headers.h>
#include <Players.h>

class verts {
public:float posx;
	   float posy;
private:
	/*friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & posx; ar & posy;
	}*/
};


class polys {
public: std::list<verts> poses;
private:
	/*friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & poses;
	}*/
};

class flyen {
public: std::list<verts> moves;
		verts starting;
		sf::CircleShape shape;
		b2Vec2 start;
		b2Vec2 end;
		//b2Body* flysim;
};

class rects {
public: float startx;
		float starty;
		float posx;
		float posy;
private:
	/*friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & startx; ar & starty; ar & posx; ar & posy;
	}*/

};

class leveltemp{
private:
	/*friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & rectlist; ar & polylist; ar & playerpos; ar & backscale; ar & endrec; ar & plpo;
	}*/
public:
	std::list<rects> rectlist;
	std::list<rects> traplist;
	std::list<polys> polylist;
	std::list<verts> plpo;
	std::list<flyen> flylist;
	verts playerpos;
	verts backscale;
	rects endrec;
	std::list<verts> wallepos;
	
};

class threadsim {
public:
	b2World* world;
	float32 tstep;
	int32 vIt;
	int32 pIt;
	void run(){
		sf::Clock clock;
		/*clock.restart();*/
		while (true){
			/*float ttime = clock.getElapsedTime().asSeconds();
			if (ttime >= 1.f / 60.f){ world->Step(ttime*20, vIt, pIt); clock.restart(); }*/
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

	  virtual ~Masterplayer(){}
};


class Playerobject{
public:
	std::string moviefile;
	sfe::Movie movie;

	sf::RenderWindow* win;
	sf::View* view;

	b2World* world;
	b2Body* playersim;
	b2WorldManifold* mani;
	sf::RectangleShape* player;
	bool slide;
	b2Body* endsens;
	std::list<b2Body*> traps;
	verts startpos;
	std::list<b2Body*> flys;
	std::list<b2Body*> walles;

	Masterplayer* master;
	int nextplayer;

	std::string backpic;
	sf::Sprite* background;

	float time;
	sf::CircleShape* clock1;
	sf::CircleShape* clock2;
	sf::Clock meter;
	float tslf;

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
	//leveltemp lvl;


	void createmovie(){
		movie.openFromFile(moviefile);
		movie.fit(0, 0, 600, 400);
		movie.play();
	}
	void updatemovie(){
		movie.update();
		if (master->keylist["spacebar"]){ movie.stop(); }
		if (movie.getStatus() != sfe::Status::Playing){ change(); return; }
	}
	void change(){
		master->change(nextplayer);
	}

	void createlevel(){
		//printf("trying file");
		leveltemp lvl;
		//std::ifstream ifs(levelfile,std::ios::binary);
		//boost::archive::binary_iarchive ia(ifs);
		//ia >> lvl; printf("madeit");
		//ifs.close();
		std::string line;
		std::ifstream tfs(levelfile);
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
			if (line == "traps"){
				rects* newrec = new rects();
				getline(tfs, line);
				newrec->posx = ::atof(line.c_str());
				getline(tfs, line);
				newrec->posy = ::atof(line.c_str());
				getline(tfs, line);
				newrec->startx = ::atof(line.c_str());
				getline(tfs, line);
				newrec->starty = ::atof(line.c_str());
				lvl.traplist.push_back(*newrec);
			}
			if (line == "walle"){
				verts* newv = new verts();
				getline(tfs, line);
				newv->posx = ::atof(line.c_str());
				getline(tfs, line);
				newv->posy = ::atof(line.c_str());
				lvl.wallepos.push_back(*newv);
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
			if (line == "flyen"){
				flyen* newfly = new flyen();
				getline(tfs, line);
				newfly->starting.posx = ::atof(line.c_str());
				getline(tfs, line);
				newfly->starting.posy = ::atof(line.c_str());
				newfly->moves.push_back(newfly->starting);
				printf("%f", newfly->starting.posy);
				while (getline(tfs, line)){
					if (line == "moves"){
						verts* newv = new verts();
						getline(tfs,line);
						newv->posx = ::atof(line.c_str());
						getline(tfs, line);
						newv->posy = ::atof(line.c_str());
						newfly->moves.push_back(*newv);
					}
					if (line == "endflyen")break;
				}
				lvl.flylist.push_back(*newfly);
				
			}
		}
		tfs.close();
		//printf("opened file");
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

		for each(flyen var in lvl.flylist){
			flyen* newfly = new flyen();
			b2BodyDef* flydef = new b2BodyDef; flydef->type = b2_staticBody;
			flydef->position.Set(var.starting.posx, var.starting.posy);
			//printf("%f,%f", var.starting.posy,flydef->position.y);
			newfly->shape.setRadius(10.f);
			newfly->shape.setFillColor(sf::Color::Green); newfly->shape.setOrigin(10.f, 10.f);
			//newfly->flysim = world->CreateBody(flydef);
			b2Body* newfsim = world->CreateBody(flydef);
			//newfsim->SetTransform(b2Vec2(var.starting.posx, var.starting.posy), 0.f);
			b2CircleShape* circ = new b2CircleShape;
			circ->m_radius = 10.f;
			newfsim->CreateFixture(circ, 1.f);
			newfsim->GetFixtureList()->SetSensor(true);
			newfly->moves = var.moves;
			//printf("%f", var.flysim->GetPosition().y);
			newfsim->SetUserData(newfly);
			flys.push_back(newfsim);
			//delete flydef;
		}

		for each(rects var in lvl.rectlist){
			//printf("%f %f %f %f", var.posx, var.posy, var.startx, var.starty);
			b2BodyDef* bodef = new b2BodyDef; bodef->type = b2_staticBody;
			bodef->position.Set(var.posx + var.startx / 2, var.posy + var.starty / 2);
			b2Body* bobo = world->CreateBody(bodef);
			b2PolygonShape* bosh = new b2PolygonShape;
			bosh->SetAsBox(var.startx / 2, var.starty / 2);
			bobo->CreateFixture(bosh, 1.0f);
		}

		for each(rects var in lvl.traplist){
			//printf("%f %f %f %f", var.posx, var.posy, var.startx, var.starty);
			b2BodyDef* bodef = new b2BodyDef; bodef->type = b2_staticBody;
			bodef->position.Set(var.posx + var.startx / 2, var.posy + var.starty / 2);
			b2Body* bobo = world->CreateBody(bodef);
			b2PolygonShape* bosh = new b2PolygonShape;
			bosh->SetAsBox(var.startx / 2, var.starty / 2);
			bobo->CreateFixture(bosh, 1.0f);
			bobo->GetFixtureList()->SetSensor(true);
			traps.push_back(bobo);
		}

		for each(verts var in lvl.wallepos){
			b2BodyDef* bodef = new b2BodyDef; bodef->type = b2_dynamicBody;
			bodef->allowSleep = false;
			bodef->fixedRotation = true;
			bodef->position.Set(var.posx, var.posy);
			b2Body* wallsim = world->CreateBody(bodef);
			b2PolygonShape* wallsh = new b2PolygonShape;
			wallsh->SetAsBox(20, 20);
			wallsim->CreateFixture(wallsh, 1.f);
			sf::RectangleShape* wallgr = new sf::RectangleShape(sf::Vector2f(40, 40));
			wallgr->setOrigin(20, 20);
			wallgr->setFillColor(sf::Color::Blue);
			wallsim->SetUserData(wallgr);
			//printf("var:%f,%f sim:%f,%f\n", var.posx, var.posy, bodef->position.x, bodef->position.y);
			//wallsim->SetTransform(b2Vec2(var.posx, var.posy), 0.f);
			walles.push_back(wallsim);
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
		startpos.posx = lvl.playerpos.posx;
		startpos.posy = lvl.playerpos.posy;
		//printf("playerpos:%f %f",playd->position.x,playd->position.y);
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
		//printf("%f %f", lvl.backscale.posx, lvl.backscale.posy);
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
		mani = new b2WorldManifold();

		threadsim* sim = new threadsim();
		sim->world = world;
		sim->tstep = 1.0f / 30000.0f;
		sim->vIt = 6;
		sim->pIt = 2;
		t1 = new sf::Thread(&forthread, sim);
		t1->launch();

	}

	void savelevel(){
		/*std::ofstream ofs("newlevel.dat",std::ios::binary);
		boost::archive::binary_oarchive oa(ofs);
		oa << lvls;
		ofs.close();*/
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
		for each(rects var in lvls->traplist){
			tfs << "traps\n";
			tfs << var.posx << "\n";
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
		for each(flyen var in lvls->flylist){
			tfs << "flyen\n";
			tfs << var.starting.posx << "\n";
			tfs << var.starting.posy << "\n";
			for each(verts vvar in var.moves){
				tfs << "moves\n";
				tfs << vvar.posx << "\n";
				tfs << vvar.posy << "\n";
			}
			tfs << "endflyen\n";
		}
		for each(verts var in lvls->wallepos){
			tfs << "walle\n";
			tfs << var.posx << "\n";
			tfs << var.posy << "\n";
		}
		tfs.close();
	}

	void updateeditor(){
		//printf("\nReal update");
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
			bltest->starty = newblock.getSize().y; //printf("%f %f %f %f", bltest->posx, bltest->posy, bltest->startx, bltest->starty);
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
			newblock.setFillColor(sf::Color(0, 0, 200, 255));
			//newblock.setOrigin((end-start)/2.f);
			tmprec.push_back(newblock);
			rects* bltest = new rects();
			bltest->posx = newblock.getPosition().x;
			bltest->posy = newblock.getPosition().y;
			bltest->startx = newblock.getSize().x;
			bltest->starty = newblock.getSize().y;
			lvls->endrec = *bltest;
		}

		if (master->keylist["trapend"]){//,
			master->insertkey("trapend", false);
			sf::Vector2f end = win->mapPixelToCoords(sf::Mouse::getPosition(*win));
			sf::Vector2f tmp;
			if (recstart.x > end.x){ tmp.x = end.x; end.x = recstart.x; recstart.x = tmp.x; }
			if (recstart.y > end.y){ tmp.y = end.y; end.y = recstart.y; recstart.y = tmp.y; }
			sf::RectangleShape newblock;
			newblock.setSize(end - recstart);
			newblock.setPosition(recstart);
			newblock.setFillColor(sf::Color(200, 0, 0, 255));
			tmprec.push_back(newblock);
			rects* bltest = new rects();
			bltest->posx = newblock.getPosition().x;
			bltest->posy = newblock.getPosition().y;
			bltest->startx = newblock.getSize().x;
			bltest->starty = newblock.getSize().y;
			lvls->traplist.push_back(*bltest);
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

		if (master->keylist["flyend"]){//u
			master->insertkey("flyend", false);
			flyen* newfly = new flyen();
			sf::Vector2f justthis = tmpvec.front(); //tmpvec.pop_front();
			newfly->starting.posx = justthis.x; newfly->starting.posy = justthis.y;
			for each(sf::Vector2f var in tmpvec){
				//newfly->starting.posx = var.x; newfly->starting.posy = var.y;
				verts* newvert = new verts();
				newvert->posx = var.x;
				newvert->posy = var.y;
				newfly->moves.push_back(*newvert);
			}
			lvls->flylist.push_back(*newfly);
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
			//printf("playerpos:%f %f", lvls->playerpos.posx, lvls->playerpos.posy);
			//int i; scanf_s("%d", &i);
		}

		if (master->keylist["walle"]){//j
			master->insertkey("walle", false);
			sf::Vector2f newv = win->mapPixelToCoords(sf::Mouse::getPosition(*win));
			verts* newvt = new verts();
			newvt->posx = newv.x;
			newvt->posy = newv.y;
			lvls->wallepos.push_back(*newvt);

		}

		if (master->keylist["scaledown"]){//k
			background->scale(0.999f, 0.999f);
			lvls->backscale.posx = background->getScale().x;
			lvls->backscale.posy = background->getScale().y;
			//printf("%f %f", lvls->backscale.posx, lvls->backscale.posy);
			//char a;
			//scanf_s("%c", &a);
		}

		if (master->keylist["scaleup"]){//L
			background->scale(1.001f, 1.001f);
			lvls->backscale.posx = background->getScale().x;
			lvls->backscale.posy = background->getScale().y;
			//printf("%f %f", lvls->backscale.posx, lvls->backscale.posy);
		}

		if (master->keylist["enter"]){
			master->insertkey("enter", false);
			savelevel();
			*view = win->getDefaultView();
			win->setView(*view);
			change(); return;
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
		//printf("\n End Real Update");


	}

	void updatelevel(){
		tslf=meter.restart().asSeconds();
		//printf("\nReal Levelupdate");
		if (endsens != NULL){
			//printf("Im closing");
			//win->close();
			if (endsens->GetContactList() != NULL){
				//printf("Touch endblock\n");
				if (endsens->GetContactList()->contact->IsTouching()){
					if (endsens->GetContactList()->other == playersim){
						t1->terminate(); delete world;
						change(); return;
					}
				}
			}
		}

		for each (b2Body* var in traps)
		{
			if (var->GetContactList() != NULL){
				if (endsens->GetContactList()->other == playersim){
					//t1->terminate(); delete world; master->music.stop(); nextplayer = 1; change();
					playersim->SetLinearVelocity(b2Vec2(0.f, 0.f));
					playersim->SetTransform(b2Vec2(startpos.posx, startpos.posy), 0.f);
				}
			}
		}

		for each(b2Body* var in flys){
			if (var->GetContactList() != NULL){
				if (endsens->GetContactList()->other == playersim){
					playersim->SetLinearVelocity(b2Vec2(0.f, 0.f));
					playersim->SetTransform(b2Vec2(startpos.posx, startpos.posy), 0.f);
				}
			}
		}
		//printf("Step1");
		if (master->keylist["enter"]){
			master->insertkey("enter", false);
			
			//printf("Shit happens");
			t1->terminate();
			delete world;
			nextplayer = 1;
			master->music.stop();
			change(); return;
		}

		if (master->keylist["up"]){
			if (playersim->GetLinearVelocity().y <= 5 && playersim->GetLinearVelocity().y >= -5 && !master->keylist["blockjump"]){
				master->insertkey("blockjump", true);
				playersim->ApplyLinearImpulse(b2Vec2(playersim->GetLinearVelocity().x, -3000), b2Vec2(playersim->GetPosition().x, playersim->GetPosition().y), true);
			}
		}

		if (master->keylist["left"]){
			slide = true;
			playersim->SetAngularVelocity(-0.5f); playersim->SetLinearVelocity(b2Vec2(-4, world->GetGravity().y*playersim->GetLinearVelocity().y));/*playersim->ApplyForceToCenter(b2Vec2(-500, 0), true);*/
			/*playersim->ApplyForce(b2Vec2(-250, 0),playersim->GetLocalCenter()-b2Vec2(0,10), true);*//*playersim->ApplyAngularImpulse(-1000, true);*/
		}

		if (master->keylist["right"]){
			slide = true;
			playersim->SetAngularVelocity(0.5f); playersim->SetLinearVelocity(b2Vec2(4, world->GetGravity().y*playersim->GetLinearVelocity().y));/*playersim->ApplyForceToCenter(b2Vec2(500, 0), true);*/
			/*playersim->ApplyForce(b2Vec2(250, 0), playersim->GetLocalCenter() - b2Vec2(0, 10), true);*//*playersim->ApplyAngularImpulse(1000, true);*/

		}

		if (master->keylist["down"]){
			if (slide){
				playersim->SetAngularVelocity(0); playersim->SetTransform(playersim->GetPosition(), 0);
				playersim->ApplyLinearImpulse(b2Vec2(playersim->GetLinearVelocity().x * 600, 100), b2Vec2(playersim->GetPosition().x, playersim->GetPosition().y), true);
				slide = false;
			}
		}
		//printf("\nStep2");
		if (master->keylist["blockjump"]){
			if (playersim->GetContactList() != NULL){
				//master->insertkey("blockjump", false);
				playersim->GetContactList()->contact->GetWorldManifold(mani);
				if (mani->normal.y <= 0){
					master->insertkey("blockjump", false);
				}
				printf("\n normal:%f", mani->normal.x);
			}
		}
		player->setPosition(playersim->GetPosition().x, playersim->GetPosition().y);
		player->setRotation(playersim->GetTransform().q.GetAngle()*180.f / 3.14156f);
		for each (b2Body *var in flys){
			//printf("%f", var->flysim->GetPosition().y);
			flyen* bla=static_cast<flyen*>(var->GetUserData());
			
			verts newmove=bla->moves.front();
			bla->start = var->GetPosition();
			bla->end.x = newmove.posx; bla->end.y = newmove.posy;
			b2Vec2 vec = bla->end - bla->start; b2Vec2 test = vec; vec.Normalize(); vec.operator*=(50.f*tslf);
			
			var->SetTransform(bla->start+vec,0.f);
			bla->shape.setPosition(var->GetPosition().x, var->GetPosition().y);
			//printf("\n%f",test.Length());
			if (test.LengthSquared()<=0.1f){
				//printf("start:%f,%f", bla->start.x, bla->start.y);
				//printf(" end:%f,%f", bla->end.x, bla->end.y);
				//printf(" vec:%f,%f\n%f", vec.x, vec.y,test.Length());
				bla->moves.push_back(bla->moves.front());
				bla->moves.pop_front();
			}
		}
		for each (b2Body *var in walles){
			sf::RectangleShape* blu = static_cast<sf::RectangleShape*>(var->GetUserData());
			blu->setPosition(var->GetPosition().x, var->GetPosition().y);
			blu->setRotation(var->GetTransform().q.GetAngle()*180.f / 3.14156f);
		}
		//b2Vec2 bla; bla.Set(5.f, 3.f); printf("%f\n%f\n%f\n", bla.Length(),bla.LengthSquared(),bla.Normalize());
		//printf("\nStep3");
		view->setCenter(player->getPosition());
		clock1->setPosition(win->mapPixelToCoords(sf::Vector2i(0, 0)));
		clock2->setPosition(clock1->getPosition() + clock2->getOrigin());
		/*clock2->rotate(0.005f*time);*/
		clock2->rotate(6.f*tslf*time);
		if (clock2->getRotation() >= 358.f){ t1->terminate(); delete world; master->music.stop(); nextplayer = 1; change(); return; }
		win->clear();
		win->setView(*view);
		//printf("\nStep4");
	}
	void draweditor(){
		//printf("\n Real drawing");
		win->draw(*background);
		for each(sf::RectangleShape var in tmprec){
			win->draw(var);
		}
		for each(sf::ConvexShape var in tmpconv){
			win->draw(var);
		}
		win->draw(tmpplayer);
		win->display();
		//printf("\n End Real drawing");
	}
	void drawlevel(){
		win->draw(*background);
		win->draw(*player);
		for each(b2Body *var in flys){
			flyen *blo = (flyen*)var->GetUserData();
			win->draw(blo->shape);
		}
		for each(b2Body *var in walles){
			sf::RectangleShape *blo = (sf::RectangleShape*)var->GetUserData();
			win->draw(*blo);
		}
		win->draw(*clock1);
		win->draw(*clock2);
		win->display();
	}
	virtual void init(){}
	virtual void update(){}
	virtual void draw(){}
	virtual ~Playerobject(){}

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
		//printf("Reached drawing");
		win->clear();
		win->draw(movie);
		win->display();
	}
	virtual ~Intromovie(){}
};

class Menuob : public Playerobject{
public:
	sf::RectangleShape menu;
	sf::Texture* menuitems;
	int curitem;

	virtual void init(){
		//printf("\nreached menuinit");
		menuitems = new sf::Texture[5];
		menuitems[0].loadFromFile("./Menu/menu1.png");
		menuitems[1].loadFromFile("./Menu/menu2.png");
		menuitems[2].loadFromFile("./Menu/menu3.png");
		menuitems[3].loadFromFile("./Menu/menu4.png");
		menuitems[4].loadFromFile("./Menu/menu5.png");
		curitem = 0;
		menu.setSize(sf::Vector2f(600, 400));
		menu.setPosition(0, 0);
		//printf("\nfinished menuinit");
	}

	virtual void update(){
		//printf("\nReached update");
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
			if (curitem == 2){
				nextplayer = -1;
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
		//printf("\nFinished update");
	}

	virtual void draw(){
		//printf("\nReached drawing");
		win->clear();
		win->draw(menu);
		win->display();
		//printf("\nFinished drawing");
	}

	virtual ~Menuob(){}
};

class Editorob : public Playerobject{
public:
	sf::Texture texts;
	sf::Texture tmptex;
	virtual void init(){
		//printf("\n Editorinitstart");
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
		//printf("\nEditorinitend");
	}
	virtual void update(){
		//printf("\nEditorupdatestart");
		updateeditor();
		//printf("\nEditorupdateend");
	}
	virtual void draw(){
		//printf("\nEditordrawstart");
		draweditor();
		//printf("\nEditordrawend");
	}
	virtual ~Editorob(){}
};

class Levelob : public Playerobject{
public:
	virtual void init(){
		//printf("\nReached Levelinit");
		createlevel();
		//printf("\nReached EndLevelinit");
	}

	virtual void update(){
		//printf("\nReached Levelupdate");
		updatelevel();
		//printf("\nReached EndLevelupdate");
	}

	virtual void draw(){
		//printf("\nReached Leveldraw");
		drawlevel();
		//printf("\nReached EndLeveldraw");
	}
	virtual ~Levelob(){}
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
			player = new Intromovie();
			player->moviefile = "Seq01.mp4";
			player->nextplayer = 1;
			music.openFromFile("./Music/M27.ogg");
			music.play();
		}
		if (i == 1){
			//printf("reached creation of menu");
			player = new Menuob();
		}
		if (i == 2){
			//printf("reached editorcreation");
			player = new Editorob();
			player->nextplayer = 1;
		}

		if (i == -1){
			player = new Levelob();
			player->nextplayer = 3;
			player->backpic = "./foreditor.png";
			player->levelfile = "./textfile.txt";
			//music.openFromFile("./Music/M27.ogg");
			//music.play();
			player->time = 0.5;
		}
		if (i == 3){
			player = new Levelob();
			player->nextplayer = 4;
			player->backpic = "./Level/level1.png";
			player->levelfile = "./Level/level1.txt";
			music.openFromFile("./Music/MGO.ogg");
			player->time = 2;
			music.play();
		}
		if (i == 4){
			player = new Levelob();
			player->nextplayer = 5;
			player->backpic = "./Level/level2.png";
			player->levelfile = "./Level/level2.txt";
			//music.openFromFile("./Music/MGO.ogg");
			player->time = 2;
			//music.play();
		}
		if (i == 5){
			player = new Levelob();
			player->nextplayer = 6;
			player->backpic = "./Level/level3.png";
			player->levelfile = "./Level/level3.txt";
			//music.openFromFile("./Music/MGO.ogg");
			player->time = 1;
			//music.play();
		}
		if (i == 6){
			player = new Levelob();
			player->nextplayer = 7;
			player->backpic = "./Level/level4.png";
			player->levelfile = "./Level/level4.txt";
			//music.openFromFile("./Music/MGO.ogg");
			player->time = 1;
			//music.play();
		}
		if (i == 7){
			player = new Levelob();
			player->nextplayer = 1;
			player->backpic = "./Level/level5.png";
			player->levelfile = "./Level/level5.txt";
			//music.openFromFile("./Music/MGO.ogg");
			player->time = 1;
			//music.play();
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
	virtual ~MPlayer(){}
};