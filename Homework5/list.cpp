void listAll(string path, const Class* c)  // two-parameter overload
{
    path += c->name();
    cout << path << endl;
    vector<Class*>::const_iterator itr;
    itr = c->subclasses().begin();
    
    if (itr != c->subclasses().end())
        path += "=>";

    for (; itr != c->subclasses().end(); itr++)
        listAll(path, *itr);
}