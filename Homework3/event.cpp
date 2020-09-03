/////////////////////////////////
//Base Class
//////////////////////////////////
class Event {
public:
    Event(string nm);
    virtual ~Event() {}
    string name() const;
    virtual bool isSport() const;
    virtual string need() const = 0;
private:
    string m_name;
};

Event::Event(string nm)
{
    m_name = nm;
}

string Event::name() const
{
    return m_name;
}

bool Event::isSport() const
{
    return false;
}

//////////////////////////////////
//Derived Classes
//////////////////////////////////

class BasketballGame : public Event {
public:
    BasketballGame(string nm);
    virtual ~BasketballGame();
    virtual bool isSport() const;
    virtual string need() const;
private:
    string m_need;
};

BasketballGame::BasketballGame(string nm)
    :Event(nm)
{
    m_need = "hoops";
}

BasketballGame::~BasketballGame()
{
    cout << "Destroying the " << name() << " basketball game" << endl;
}

bool BasketballGame::isSport() const
{
    return true;
}

string BasketballGame::need() const
{
    return m_need;
}

class Concert : public Event {
public:
    Concert(string nm, string genre);
    virtual ~Concert();
    virtual string need() const;
private:
    string m_need;
    string m_genre;
};

Concert::Concert(string nm, string genre)
    :Event (nm)
{
    m_need = "a stage";
    m_genre = genre;
}

Concert::~Concert()
{
    cout << "Destroying the " << name() << " " << m_genre << " concert" << endl;
}

string Concert::need() const
{
    return m_need;
}

class HockeyGame : public Event
{
public:
    HockeyGame(string nm);
    virtual ~HockeyGame();
    virtual bool isSport() const;
    virtual string need() const;
private:
    string m_need;
};

HockeyGame::HockeyGame(string nm)
    :Event(nm)
{
    m_need = "ice";
}

HockeyGame::~HockeyGame()
{
    cout << "Destroying the " << name() << " hockey game" << endl;
}

bool HockeyGame::isSport() const
{
    return true;
}

string HockeyGame::need() const
{
    return m_need;
}
