#include "randomforest.h"
#include <QTextStream>

RandomForest::RandomForest(const QString &_name) :
    m_name(_name),
    m_leafs(0)
{

}

void RandomForest::exportForest(const QStringList &_features, const QString &_rawData, const QString &_path)
{
    m_leafs = 0;

    QString data;
    QTextStream stream(&data);

    QStringList trees = _rawData.split("RandomTree\n==========");
    trees.removeFirst();

    stream << "#ifndef " << m_name.toUpper() << "_H\n";
    stream << "#define " << m_name.toUpper() << "_H\n\n";

    // individual trees
    for(int i=0; i<trees.size(); i++)
    {
        stream << "float " << m_name << "_tree" << i << "(";
        for(int j=0; j<_features.size(); j++)
        {
            stream << "float " << _features.at(j);
            if(j<_features.size()-1)
                stream << ", ";
        }
        stream << ")\n{\n";
        stream << parseTree(trees.at(i));
        stream << "}\n\n";
    }

    // majority vote
    stream << "float " << m_name << "_prediction(";
    for(int j=0; j<_features.size(); j++)
    {
        stream << "float " << _features.at(j);
        if(j<_features.size()-1)
            stream << ", ";
    }
    stream << ")\n{\n";
    stream << "\tfloat prediction = 0.0;\n";
    for(int i=0; i<trees.size(); i++)
    {
        stream << "\tprediction += " << m_name << "_tree" << i << "(";
        for(int j=0; j<_features.size(); j++)
        {
            stream << _features.at(j);
            if(j<_features.size()-1)
                stream << ", ";
        }
        stream << ");\n";
    }
    stream << "\treturn prediction/" << trees.size() << ".0;\n";
    stream << "}\n\n";


    stream << "#endif //" << m_name.toUpper() << "_H";



    WEKA::write(data, _path);

    qDebug() << "RandomForest::exportForest" << m_leafs;

}

QString RandomForest::parseTree(const QString &_tree)
{
    QString data;
    QTextStream stream(&data);

    QString tree = _tree.split("Size of the tree").first();
    QStringList lines = tree.split("\n");
    for(int i=0; i<lines.size(); i++)
    {
        QString line = lines.at(i);
        if(line.simplified().size()==0)
            continue;

        //
        int depth = getDepth(line);
        for(int j=0; j<depth; j++)
            stream << "\t";
        line = line.split("|").last().simplified();


        int deltaDepth = 0;
        if(i>0)
        {
            deltaDepth = depth - getDepth(lines.at(i-1));

            if(lines.at(i-1).simplified().size()==0)
                deltaDepth = 1;
        }

        if(line.contains(":"))
        {
            // result line
            stream << parseIfCondition(line, deltaDepth) << "\n";
            for(int j=0; j<depth+1; j++)
                stream << "\t";
            stream << parseResultStatement(line);

            m_leafs++;
        }
        else
        {
            stream << parseIfCondition(line, deltaDepth);
        }

        if(i<lines.size()-1)
        {
            int delta = getDepth(lines.at(i+1)) - depth;
            if(delta>0)
            {
                stream << "{";
            }
            else if(delta==0)
            {

            }
            else
            {
                for(int j=0; j<abs(delta); j++)
                {
                    stream << "\n";
                    for(int k=0; k<depth-j-1; k++)
                        stream << "\t";

                    stream << "}";
                }

            }

        }


        stream << "\n";
    }

    return data;
}

QString RandomForest::parseIfCondition(const QString &_line, int _deltaDepth)
{
    QString data = "if(" + _line.split(" :").first() + ")";
    if(_deltaDepth<1)
        data = "else";

    return data.simplified();
}

QString RandomForest::parseResultStatement(const QString &_line)
{
    QString data;
    QTextStream stream(&data);

    QString line = _line.split(":").last().split(" (").first();
    stream << "return" << line << ";";

    return data;
}

int RandomForest::getDepth(const QString &_line)
{
    return _line.split("|").size();
}


