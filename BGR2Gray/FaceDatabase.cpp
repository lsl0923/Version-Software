#include "FaceDatabase.h"
#include <cmath>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

bool FaceDatabase::load(const QString& filename) {
    QFile file(filename);
    // 如果文件不存在，则创建一个空的 JSON 数组文件
    if (!file.exists()) {
        if (!file.open(QIODevice::WriteOnly)) return false;
        QJsonDocument emptyDoc{QJsonArray()};
        file.write(emptyDoc.toJson());
        file.close();
    }

    // 打开文件读取
    if (!file.open(QIODevice::ReadOnly)) return false;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return false;

    QJsonArray array = doc.array();

    records.clear();
    for (const QJsonValue& val : array) {
        QJsonObject obj = val.toObject();
        FaceRecord rec;
        rec.name = obj["name"].toString();
        QJsonArray feat = obj["feature"].toArray();
        for (auto v : feat) rec.feature.push_back(static_cast<float>(v.toDouble()));
        records.push_back(rec);
    }

    return true;
}

bool FaceDatabase::save(const QString& filename) {
    // Step 1: 读取已有 JSON 数据（如果存在）
    QJsonArray existingArray;
    QFile file_in(filename);
    if (file_in.exists() && file_in.open(QIODevice::ReadOnly)) {
        QByteArray data = file_in.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isArray())
            existingArray = doc.array();
        file_in.close();
    }

    // Step 2: 构建 name -> index 映射，便于查重更新
    QMap<QString, int> nameToIndex;
    for (int i = 0; i < existingArray.size(); ++i) {
        QJsonObject obj = existingArray[i].toObject();
        nameToIndex[obj["name"].toString()] = i;
    }

    // Step 3: 遍历 records，更新或追加
    for (const FaceRecord& rec : records) {
        QJsonObject obj;
        obj["name"] = rec.name;

        QJsonArray feat;
        for (float v : rec.feature)
            feat.append(v);
        obj["feature"] = feat;

        if (nameToIndex.contains(rec.name)) {
            // 更新原记录
            existingArray[nameToIndex[rec.name]] = obj;
        } else {
            // 追加新记录
            existingArray.append(obj);
        }
    }

    // Step 4: 将更新后的 array 写回文件
    QFile file_out(filename);
    if (!file_out.open(QIODevice::WriteOnly)) return false;
    QJsonDocument outDoc(existingArray);
    file_out.write(outDoc.toJson());
    file_out.close();

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
