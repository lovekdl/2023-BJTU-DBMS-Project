#include "database.h"
#include<assert.h>
Database::Database() {

}

Database::Database(std::string database_name, std::string owner_user) {
    this->database_name = database_name;
    this->owner_user = owner_user;
}

const std::string& Database::GetOwnerUserName() const {
    return owner_user;
}

const std::string& Database::GetDatabaseName() const {
    return database_name;
}

const std::vector<Table>& Database::GetTables() const {
    return tables;
}

void Database::SetTables(const std::vector<Table>& tables) {
    this->tables = tables;
}
int Database::FindTable(std::string table_name) {
    for(const auto& table : tables) {
        if(table.GetTableName() == table_name) return kSuccess;
    }
    return kTableNotFound;
}

Table& Database::FindTableReference(std::string table_name) {
    for(auto& table : tables) {
        if(table.GetTableName() == table_name) return table;
    }
    assert(1);
    //impossible situation
        return tables[0];
}

int Database::FindTable(std::string table_name, Table& return_table) {
    for(const auto& table : tables) {
        if(table.GetTableName() == table_name) {
            return_table = table;
            return kSuccess;
        }
    }
    return kTableNotFound;
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
int Database::AlterTableConstraint(std::string table_name, Constraint* constraint) {
    for(auto& table:tables) {
        if(table.GetTableName() == table_name) {
            if(dynamic_cast<const ForeignKeyConstraint *>(constraint) != nullptr) {
                ForeignReferedConstraint* refered_constraint = new ForeignReferedConstraint(dynamic_cast<const ForeignKeyConstraint *>(constraint)->GetReferenceFieldName(),table_name,constraint->GetFieldName());
                int ret = AlterTableConstraint(dynamic_cast<const ForeignKeyConstraint *>(constraint)->GetReferenceTableName(),refered_constraint);
                if(ret!=kSuccess) return ret;
            }
            return table.AlterTableConstraint(constraint);
        }
    }
    
    return kTableNotFound;
}

int Database::DropTable(std::string table_name) {
    for(int i = 0; i < tables.size(); ++i) {
        if(tables[i].GetTableName() == table_name) {

            //check constraint
            auto constraints = tables[i].GetConstraints();
            for(const auto& constraint : constraints) {
                if(dynamic_cast<const ForeignReferedConstraint *>(constraint) != nullptr){return kBeingRefered;}
            }
            
            //删除所有被此表参考的约束
            for(const auto& constraint : constraints) {
                if(dynamic_cast<const ForeignKeyConstraint *>(constraint) != nullptr){
                    std::string reference_table_name = dynamic_cast<const ForeignKeyConstraint *>(constraint)->GetReferenceTableName();
                    std::string tmp;
                    FindTableReference(reference_table_name).DropForeignReferedConstraint(table_name);
                }
            }
            tables.erase(tables.begin() + i);
            return kSuccess;
        }
    }
    return kTableNotFound;
}

int Database::Insert(std::string table_name, std::vector<std::pair<std::string, std::string>> record_in) {
    for(auto& table : tables) {
        if(table.GetTableName() == table_name) {
            return table.Insert(record_in, this);
        }
    }
    return kTableNotFound;
}

int Database::Delete(std::string table_name, std::vector<std::tuple<std::string, std::string, int>> conditions) {
    for(auto& table : tables) {
        if(table.GetTableName() == table_name) {
            return table.Delete(conditions, this);
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
            return table.Update(values, conditions, this);
        }
    }
    return kTableNotFound;
}

int Database::DescribeTable(std::string table_name,std::vector<std::pair<std::string, std::string>>& fields,std::vector<Constraint*>& constraints) {
    for(auto& table: tables) {
        if(table.GetTableName() == table_name) {
            return table.DescribeTable(fields, constraints);

        }
    }
    return kTableNotFound;
}

int Database::AlterTableAdd(std::string table_name, std::pair<std::string, std::string> field) {
    for(auto& table:tables) {
        if(table.GetTableName() == table_name) {
            return table.AlterTableAdd(field);
        }
    }
    return kTableNotFound;
}

int Database::AlterTableDrop(std::string table_name, std::string field_name) {
    for(auto& table:tables) {
        if(table.GetTableName() == table_name) {
            int ret= table.AlterTableDrop(field_name, this);
            
            return ret;
        }
    }
    return kTableNotFound;
}

int Database::AlterTableModify(std::string table_name, std::pair<std::string, std::string> field){
    for(auto& table :tables) {
        if(table.GetTableName() == table_name) {
            int ret = table.AlterTableModify(field);
            //std::cout<<"ret is "<<ret<<std::endl;
            return ret;
        }
    }
    return kTableNotFound;
};

int Database::ShowTables(std::vector<std::string>& return_tables) {
    for(const auto& table:tables) {
        return_tables.push_back(table.GetTableName());
    }
    return kSuccess;
}

int Database::BuildIndex(std::string table_name, const std::vector<std::string>& compare_key) {
    for(auto& table: tables) {
        if(table.GetTableName() != table_name) continue;
        const auto& fmap = table.GetFieldMap();
        for(const auto& key: compare_key) {
            if(fmap.count(key) == 0) {
                return kFieldNotFound;
            }
        }
        return table.BuildIndex(compare_key);
    }
    return kTableNotFound;

}

