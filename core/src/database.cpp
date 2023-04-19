#include "database.h"

Database::Database() {

}

Database::Database(std::string database_name, std::string owner_user) {
    this->database_name = database_name;
    this->owner_user = owner_user;
}

std::string Database::GetOwnerUserName() {
    return owner_user;
}

std::string Database::GetDatabaseName() {
    return database_name;
}

int Database::CreateTable(std::string table_name, std::vector<std::pair<std::string, std::string>> fields, std::vector<Constraint *> constraints) {
    for(const auto& table : tables) {
        if(table.GetTableName() == table_name) {
            return kTableNameExisted;
        }
    }
    tables.push_back(Table(table_name, fields, constraints));
    return kSuccess;
}

int Database::DropTable(std::string table_name) {
    for(int i = 0; i < tables.size(); ++i) {
        if(tables[i].GetTableName() == table_name) {
            tables.erase(tables.begin() + i);
            return kSuccess;
        }
    }
    return kTableNotFound;
}

int Database::Insert(std::string table_name, std::vector<std::pair<std::string, std::string>> record_in) {
    for(auto& table : tables) {
        if(table.GetTableName() == table_name) {
            return table.Insert(record_in);
        }
    }
    return kTableNotFound;
}

int Database::Delete(std::string table_name, std::vector<std::tuple<std::string, std::string, int>> conditions) {
    for(auto& table : tables) {
        if(table.GetTableName() == table_name) {
            return table.Delete(conditions);
        }
    }
    return kTableNotFound;
}

int Database::Select(std::string table_name, std::vector<std::string> field_name, std::vector<std::tuple<std::string, std::string, int>> conditions, std::vector<std::vector<std::any>> &return_records) {
    for(auto& table : tables) {
        if(table.GetTableName() == table_name) {
            return table.Select(field_name, conditions, return_records);
        }
    }
    return kTableNotFound;
}
int Database::Update(std::string table_name, const std::vector<std::pair<std::string,std::string>>& values, const std::vector<std::tuple<std::string, std::string, int>>& conditions) {
    for(auto& table:tables) {
        if(table.GetTableName() == table_name) {
            return table.Update(values, conditions);
        }
    }
    return kTableNotFound;
}