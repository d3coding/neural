#include "headers/tools.hpp"
#include "headers/matrix.hpp"
#include <iostream>
#include <cstdlib>
#include <chrono>

Tools::Tools(string filePath){
  try{
    cout << "# Trying to load the file on \"" << filePath << "\" ...\n";
    if(loadPreset(filePath.c_str()))
      cout << "# Loaded...\n";
    else throw(0);
  } 
  catch(int x){
    switch(x){
      case 251:
        cout << "# ERR # DATA SAMPLE NOT FOUND!\n";
        exit(255);
        break;
      default:
        cout << "# ERR # NOT LOADED...\n";
        exit(250);
        break;
    }
  }
}

Tools::~Tools(){
  if(m != NULL)
    delete m;
}

void Tools::defIterations(int x){
  iterations = x;
}

void Tools::learn(bool sElapsed){
  if(iterations == 0){
    cout << "# ERR # ITERATIONS ARE 0!\n" << endl;
    exit(240);
  } else {
    auto start = std::chrono::system_clock::now();
    cout << "# Training...\n";
    m->learn(iterations);
    auto end = chrono::system_clock::now();
    cout << "# Sucess on Training!...\n";
    if(sElapsed){
      auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
      cout << "# Elapsed Time: " << elapsed.count() << "ms\n";
    }
  }
}

#ifdef __linux

#include <thread>

void Tools::tLearn(bool sElapsed){
	mThread = new thread(&Tools::learn,this,sElapsed);
}

void Tools::tJoin(){
  if(mThread == NULL){
    cout << "# ERR # No thread available!\n" << endl;
    exit(230);
  } else if(mThread->joinable()){
    cout << "# Joining...\n";
    mThread->join();
    cout << "# Joined!...\n";
    } else {
    	cout << "# ERR # Not joinable!\n" << endl;
    	exit(231);
    }
}

void Tools::tStop(){
	if(mThread == NULL){
    cout << "# ERR # No thread available!\n" << endl;
    exit(230);
  } else if(mThread->joinable()){
  	m->stop();
      mThread->join();
  }
}

#endif //__linux

void Tools::addInputs(double value){
  cout << "# Adding input value(" << value << ") at(" << input.size() << ")...\n";
  input.push_back(value);
}

void Tools::addInputs(double *value[]){
  MatrixData *data = m->getAllData();
  for(unsigned int x(0);x < data->Layer.front();++x){
    cout << "# Adding input value(" << value << ") at(" << input.size() << ")...\n";
    input.push_back(*value[x]);
    delete value[x];
  }
  delete data;
}

void Tools::setMatrix(Matrix *newMatrix){
  m = newMatrix;
}

Matrix* Tools::getMatrix(){
  return m;
}

void Tools::makeCalc(bool ask,bool logic){
	if(ask){
		input.clear();
		MatrixData *data = m->getAllData();
		for(unsigned int x(0);x < data->Layer.front();++x){
			cout << "# Input for " << x << " neuron:";
			double y;
			cin >> y;
			input.push_back(y);
		}
		delete data;
	} else {
  cout << "# Calculating with added input values...\n";
  }
  vector<double> result = m->calculate(input);
  cout << "# Showing results:\n";
  if(logic){
  for(unsigned int x(0);x < result.size();++x){
    cout << "# \tR" << x << ": " << ( !(result.at(x) < .5) ? "true" : "false") << endl;
  }
}else {
  	for(unsigned int x(0);x < result.size();++x){
    cout << "# \tR" << x << ": " << result.at(x) << endl;
  }
  }
}

void Tools::printResult() {
  MatrixData *tmp = m->getAllData();
  cout << "##### Weight ######\n";
  for(unsigned int x(0); x < tmp->Weight.size(); ++x) {
    for(unsigned int y(0); y < tmp->Weight.at(x).size(); ++y)
      cout << "[" << x << "," << y << "] " << *tmp->Weight.at(x).at(y) << "\t";
    cout << endl;
  }
  cout << "\n##### Bias #####\n";
  for(unsigned int x(0); x < tmp->Bias.size(); ++x) {
    for(unsigned int y(0); y < tmp->Bias.at(x).size(); ++y)
      cout << "[" << x << "," << y << "] " << *tmp->Bias.at(x).at(y) << "\t";
    cout << "\n";
  }
  cout << "\n##### Expected #####\n";
  for(unsigned int x(0);x < tmp->Data.size();++x){
    for(unsigned int y(0);y < tmp->Layer.back();++y)
      cout << "EXPEC(" << x << "):\t " << tmp->Data.at(x).at(1).at(y) << "\t";
    cout << "\n";
    for(unsigned int y(0);y < tmp->Layer.back();++y)
      cout << "ERROR(" << x << "):\t " << tmp->Data.at(x).at(1).at(y) - tmp->Data.at(x).back().at(y) << "\t";
    cout << "\n";
  }
  cout << "\n##### ERROR #####\n";
  double tot(0.0);
  int n(0);
  for(unsigned int x(0);x < tmp->Data.size();++x){
  	for(unsigned int y(0);y < tmp->Layer.back();++y){
      tot += abs(tmp->Data.at(x).at(1).at(y) - tmp->Data.at(x).back().at(y));
      ++n;
    }
  }
  cout << "\nTOTAL ERROR: " << tot << endl;
  cout << "\nPERCENT ERROR: " << (100*tot)/n << endl;
}

void Tools::setLogicGate(double a,double b,double c,double d){
  vector< vector< vector<double> > > data;
  for(int x(0); x < 4; ++x) {
    vector< vector<double> > a;
    data.push_back(a);
    for(unsigned int y(0);y < 2;++y){
      vector<double> a;
      data.at(x).push_back(a);
    }
  }
  {
    {
      data.at(0).at(0).push_back(0);
      data.at(0).at(0).push_back(0);
      data.at(0).at(1).push_back(a);
    }{
      data.at(1).at(0).push_back(0);
      data.at(1).at(0).push_back(1);
      data.at(1).at(1).push_back(b);
    }{
      data.at(2).at(0).push_back(1);
      data.at(2).at(0).push_back(0);
      data.at(2).at(1).push_back(c);
    }{
      data.at(3).at(0).push_back(1);
      data.at(3).at(0).push_back(1);
      data.at(3).at(1).push_back(d);
    }
  }
  for(unsigned int x(0); x < data.size(); ++x)
    m->addData(data.at(x));
}

#include <fstream>

bool Tools::saveFile(string fileName,bool show){
  fileName = "./" + fileName + ".txt";
  ofstream file(fileName.c_str());
  if(file.is_open()){
    MatrixData *data = m->getAllData();
    // Matrix
    file << "{{";
    for(unsigned int x(0);x < data->Layer.size();++x){
      file << "{" << data->Layer.at(x) << "}";
    }
    file << "}";
    // Weight
    file << "{";
    for(unsigned int x(0);x < data->Weight.size();++x){
      file << "{";
      for(unsigned int y(0);y < data->Weight.at(x).size();++y)
        file << "{" << *data->Weight.at(x).at(y) << "}";
      file << "}";
    }
    file << "}";
    // Bias
    file << "{";
    for(unsigned int x(0);x < data->Bias.size();++x){
      file << "{";
      for(unsigned int y(0);y < data->Bias.at(x).size();++y)
        file << "{" << *data->Bias.at(x).at(y) << "}";
      file << "}";
    }
    file << "}";
    // Data
    file << "{";
    for(unsigned int x(0);x < data->Data.size();++x){
      file << "{";
      for(unsigned int y(0);y < 2;++y){
        file << "{";
        for(unsigned int z(0);z < data->Data.at(x).at(y).size();++z){
          file << "{" << data->Data.at(x).at(y).at(z) << "}";
        }
        file << "}";
      }
      file << "}";
    }
    file << "}";
    // iterations
    file << "{" << iterations << "}}";
    file.close();
    delete data;
    return 0;
  } else{
    return 1;
  }
}

bool Tools::loadFile(string fileName){
  return 1;
}

#ifdef QT_XML_LIB

#include <QDomDocument>
#include <QTextStream>
#include <QDomElement>
#include <QDomAttr>
#include <QString>
#include <QFile>

bool Tools::savePreset(string presetName, bool show){
  MatrixData *dat = m->getAllData();
  QDomDocument doc;
  QDomElement Network = doc.createElement("Network");
  const QString QpresetName  = QString::fromStdString(presetName);
  Network.setAttribute((QString)"presetName",QpresetName);
  doc.appendChild(Network);
  QDomElement Matrix = doc.createElement("Matrix");
  for(unsigned int x(0);x < dat->Layer.size();++x){
    QDomElement Layer = doc.createElement("Layer");
    Layer.appendChild(doc.createTextNode(to_string(dat->Layer.at(x)).c_str()));
    Matrix.appendChild(Layer);
  }
  Network.appendChild(Matrix);

  QDomElement Weight = doc.createElement("Weight");
  for(unsigned int x(0);x < dat->Weight.size();++x){
    QDomElement WeightR = doc.createElement("WeightR");
    for(unsigned int y(0);y < dat->Weight.at(x).size();++y){
      QDomElement WeightC = doc.createElement("WeightC");
      WeightC.appendChild(doc.createTextNode(to_string(*dat->Weight.at(x).at(y)).c_str()));
      WeightR.appendChild(WeightC);
    }
    Weight.appendChild(WeightR);
  }
  Network.appendChild(Weight);

  QDomElement Bias = doc.createElement("Bias");
  for(unsigned int x(0);x < dat->Bias.size();++x){
    QDomElement BiasR = doc.createElement("BiasR");
    for(unsigned int y(0);y < dat->Bias.at(x).size();++y){
      QDomElement BiasC = doc.createElement("BiasC");
      BiasC.appendChild(doc.createTextNode(to_string(*dat->Bias.at(x).at(y)).c_str()));
      BiasR.appendChild(BiasC);
    }
    Bias.appendChild(BiasR);
  }
  Network.appendChild(Bias);

  QDomElement Data = doc.createElement("Data");
  for(unsigned int x(0);x < dat->Data.size();++x){
    QDomElement dataSet = doc.createElement("dataSet");
    QDomElement Input = doc.createElement("Input");
    for(unsigned int y(0);y < dat->Data.at(x).at(0).size();++y){
      QDomElement value = doc.createElement("Value");
      value.appendChild(doc.createTextNode(to_string(dat->Data.at(x).at(0).at(y)).c_str()));
      Input.appendChild(value);
    }
    dataSet.appendChild(Input);

    QDomElement Expected = doc.createElement("Expected");
    for(unsigned int y(0);y < dat->Data.at(x).at(1).size();++y){
      QDomElement value = doc.createElement("Value");
      value.appendChild(doc.createTextNode(to_string(dat->Data.at(x).at(1).at(y)).c_str()));
      Expected.appendChild(value);
    }
    dataSet.appendChild(Expected);
    Data.appendChild(dataSet);
  }
  Network.appendChild(Data);

  QDomElement Iterations = doc.createElement("Iterations");
  Iterations.appendChild(doc.createTextNode(to_string(iterations).c_str()));
  Network.appendChild(Iterations);

  QString fileName = "./" + QpresetName + ".xml";
  QFile file(fileName.toStdString().c_str());
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
    return 0;
  }
  QTextStream out(&file);
  out << doc.toString();
  if(show) cout << "\n##################################################\n\n"
                << doc.toString().toStdString()
                << "\n##################################################\n\n";
  file.flush();
  file.close();
  delete dat;
  return 1;
}

bool Tools::loadPreset(string fileDirectory){
  MatrixData *dat = new MatrixData;
  QFile file(fileDirectory.c_str());
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
    return 0;
  }
  QDomDocument doc;
  doc.setContent(&file);
  file.close();

  QDomElement Network = doc.firstChild().toElement();
  dat->presetName = Network.attribute("presetName").toStdString();
  QDomElement Layer = Network.firstChild().firstChild().toElement();
  while(!Layer.isNull()){
    dat->Layer.push_back(Layer.toElement().text().toInt());
    Layer = Layer.nextSibling().toElement();
  }

  QDomElement WeightR = Network.firstChild().nextSibling().firstChild().toElement();
  while(!WeightR.isNull()){
    vector<double*> a;
    dat->Weight.push_back(a);
    QDomElement WeightC = WeightR.firstChild().toElement();
    while(!WeightC.isNull()){
      dat->Weight.back().push_back(new double(WeightC.toElement().text().toDouble()));
      WeightC = WeightC.nextSibling().toElement();
    }
    WeightR = WeightR.nextSibling().toElement();
  }

  QDomElement BiasR = Network.firstChild().nextSibling().nextSibling().firstChild().toElement();
  while(!BiasR.isNull()){
    vector<double*> a;
    dat->Bias.push_back(a);
    QDomElement BiasC = BiasR.firstChild().toElement();
    while(!BiasC.isNull()){
      dat->Bias.back().push_back(new double(BiasC.toElement().text().toDouble()));
      BiasC = BiasC.nextSibling().toElement();
    }
    BiasR = BiasR.nextSibling().toElement();
  }

  QDomElement dataSet = Network.firstChild().nextSibling().nextSibling().nextSibling().firstChild().toElement();
  while(!dataSet.isNull()){
    vector< vector<double> > a;
    dat->Data.push_back(a);
    vector<double> b;
    vector<double> c;
    dat->Data.back().push_back(b);
    dat->Data.back().push_back(c);
    {
      QDomElement Value = dataSet.firstChild().firstChild().toElement();
      while(!Value.isNull()){
        dat->Data.back().front().push_back(Value.toElement().text().toDouble());
        Value = Value.nextSibling().toElement();
      }
    }{
      QDomElement Value = dataSet.firstChild().nextSibling().firstChild().toElement();
      while(!Value.isNull()){
        dat->Data.back().back().push_back(Value.toElement().text().toFloat());
        Value = Value.nextSibling().toElement();
      }
    }
    dataSet = dataSet.nextSibling().toElement();
  }

  iterations = Network.firstChild().nextSibling().nextSibling().nextSibling().toElement().text().toInt();
  m = new Matrix(dat);
  delete dat;
  return 1;
}

#else

bool Tools::savePreset(string presetName, bool show){
  cout << "# ERR # QT IS NOT AVALIABLE\n";
  cout << "#  !  # Try to use: saveFile(\"same arguments\")\n";
  return 0;
}

bool Tools::loadPreset(string fileDirectory){
  cout << "# ERR # QT IS NOT AVALIABLE\n";
  cout << "#  !  # Try to use: loadFile(\"same arguments\")\n";
  return 0;
}

#endif
