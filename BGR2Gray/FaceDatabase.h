#ifndef FACEDATABASE_H
#define FACEDATABASE_H

#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <vector>

struct FaceRecord {
    QString name;
    std::vector<float> feature;
};

class FaceDatabase {
public:
    bool load(const QString& filename);
    bool save(const QString& filename);
    void add(const FaceRecord& record);
    bool findMatch(const std::vector<float>& feature, FaceRecord& bestMatch, float& bestScore, float threshold = 0.5f);


    std::vector<FaceRecord> records;

    float cosineSimilarity(const std::vector<float>& a, const std::vector<float>& b);
};

#endif // FACEDATABASE_H
