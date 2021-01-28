typedef map<string,string> ConsoleModel;

Input consoleInput(ConsoleModel model){
    Input input = {};
    for (auto it = model.begin(); it != model.end(); ++it)
    {
        string fieldName = it->first;

        cout << it->second;
        string field;
        cin >> field;

        input.insert({fieldName,string(field)});
    }
    return input;
}