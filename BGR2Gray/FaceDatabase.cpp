#include "FaceDatabase.h"
#include <cmath>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

bool FaceDatabase::load(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return false;

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return false;

    records.clear();
    QJsonArray arr = doc.array();
    for (const auto& value : arr)
    {
        QJsonObject obj = value.toObject();
        FaceRecord record;
        record.name = obj["name"].toString();

        // 全局特征
        QJsonArray featArray = obj["feature"].toArray();
        for (const auto& val : featArray) {
            record.feature.push_back(static_cast<float>(val.toDouble()));
        }

        // 局部特征
        if (obj.contains("localFeats"))
        {
            QJsonArray localFeatsArray = obj["localFeats"].toArray();
            for (const auto& featItem : localFeatsArray)
            {
                QJsonArray single = featItem.toArray();
                std::vector<float> local;
                for (const auto& val : single)
                {
                    local.push_back(static_cast<float>(val.toDouble()));
                }
                record.localFeats.push_back(local);
            }
        }

        records.push_back(record);
    }
    return true;
}

bool FaceDatabase::save(const QString& filePath)
{
    QJsonArray arr;
    for (const auto& record : records) {
        QJsonObject obj;
        obj["name"] = record.name;

        // 全局特征
        QJsonArray featArray;
        for (float v : record.feature)
        {
            featArray.append(v);
        }
        obj["feature"] = featArray;

        // 局部特征数组
        QJsonArray localFeatsArray;
        for (const auto& local : record.localFeats)
        {
            QJsonArray oneFeat;
            for (float val : local) {
                oneFeat.append(val);
            }
            localFeatsArray.append(oneFeat);
        }
        obj["localFeats"] = localFeatsArray;

        arr.append(obj);
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) return false;
    file.write(QJsonDocument(arr).toJson());
    return true;
}
void FaceDatabase::add(const FaceRecord& record) {
    records.push_back(record);
}

bool FaceDatabase::findMatch(const std::vector<float>& feature, FaceRecord& bestMatch, float& bestScore, float threshold) {
    bestScore = -1.0f;
    for (const auto& rec : records) {
        float score = cosineSimilarity(feature, rec.feature);
        if (score > bestScore) {
            bestScore = score;
            bestMatch = rec;
        }
    }
    return bestScore >= threshold;
}

float FaceDatabase::cosineSimilarity(const std::vector<float>& a, const std::vector<float>& b) {
    if (a.size() != b.size()) return 0.0f;
    float dot = 0, normA = 0, normB = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        dot += a[i] * b[i];
        normA += a[i] * a[i];
        normB += b[i] * b[i];
    }
    return dot / (std::sqrt(normA) * std::sqrt(normB) + 1e-6f);
}
