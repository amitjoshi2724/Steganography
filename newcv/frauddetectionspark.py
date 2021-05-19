import pandas as pd
import pyspark as pys
from pyspark import SparkContext, SparkConf
from pyspark.ml.classification import MultilayerPerceptronClassifier

class fraudDetectionSpark():
    #Parameters
    seed = 123
    learningRate = 0.0001
    batchSize = 200
    nEpochs = 100
    numInputs = 24
    numOutputs = 2
    numHiddenNodes = 20

def main():

    #Read in the data from a csv file, converts into a dataFrame
        #The nth row in the csv file is .loc[n] in this dataFrame
    sc = SparkContext()
    #Use .take(n) to access the data, n starts from 1
    data = sc.textFile('german_data.csv')
    #Define layers for the neural network
    layers = [24,20,20,20,20]
    FNN = MultilayerPerceptronClassifier(maxIter=100, layers=layers, blockSize=128, seed=1234)
    
main()