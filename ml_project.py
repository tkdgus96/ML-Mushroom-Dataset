import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.model_selection import KFold
from sklearn.feature_selection import RFE
from sklearn.ensemble import RandomForestClassifier
from sklearn.neural_network import MLPClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.tree import DecisionTreeClassifier
from sklearn.preprocessing import LabelEncoder
from sklearn.naive_bayes import CategoricalNB
from sklearn.model_selection import cross_val_score
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans



data_url = "https://archive.ics.uci.edu/ml/machine-learning-databases/mushroom/agaricus-lepiota.data"

mushroom_data = pd.read_csv(data_url)

mushroom_data.columns = ["Class", "Cap-shape", "Cap-surface", "Cap-color", "Bruises", "Odor", "gill-attachment", "gill-spacing", "gill-size", "gill-color", "stalk-shape", "stalk-root", "stalk-surface-above-ring", "stalk-surface-below-ring", "stalk-color-above-ring", "stalk-color-below-ring", "veil-type", "veil-color", "ring-number", "ring-type", "spore-print-color", "population", "habitat"]

mushroom_data.head()

x=mushroom_data.drop(columns=['Class'])
x= pd.get_dummies(x)
y=mushroom_data[['Class']]
le = LabelEncoder()
y.Class=le.fit_transform(y.Class)

#데이터 셋을 Training, Test로 분리
x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.3)


rfc1 = RandomForestClassifier(max_depth=6)
cnb1 = CategoricalNB(alpha=1.0)
mlp1 = MLPClassifier(max_iter=3)
lgr1 = LogisticRegression(max_iter=3)
dtc1 = DecisionTreeClassifier(max_depth=6)


lgr1.fit(x_train, y_train)
lgrscores = cross_val_score(lgr1, x_train, y_train, cv= 5)
print('LogisticRegression Cross Validation Attempt 1: '+str(lgrscores))
#[0.9516696  0.94898857 0.95514512 0.941073   0.93227792]

lgr2 = LogisticRegression(max_iter=15)
lgr2.fit(x_train, y_train)
lgrscores = cross_val_score(lgr2, x_train, y_train, cv= 5)
print("LogisticRegression Cross Validation Attempt 2: "+str(lgrscores))
#max_iter 값을 15로 증가 시켰을 때 Cross-Val-Score가 1.0까지 증가하는 것을 볼 수 있습니다.
#[1.         1.         1.         1.         0.99736148]

dtc1.fit(x_train, y_train)
dtc1.score(x_test, y_test)
dtcscores = cross_val_score(dtc1, x_train, y_train, cv= 5)
print("DecisionTreeClassifier Cross Validation Attempt 1: "+str(dtcscores))
#max_depth값 4로 작을 경우 Cross-Val-Score가 0.993이 나왔습니다.
#[0.99912127 1.         1.         1.         1.        ]

dtc2 = DecisionTreeClassifier(max_depth=7)
dtc2.fit(x_train, y_train)
dtcscores = cross_val_score(dtc2, x_train, y_train, cv= 5)
print("DecisionTreeClassifier Cross Validation Attempt 2: "+str(dtcscores))
#max_depth 값을 7까지 늘리면 Cross-Val-Score가 1.0까지 증가하는 것을 볼 수 있습니다.
#[1. 1. 1. 1. 1.]

mlp1.fit(x_train, y_train)
mlpscores = cross_val_score(mlp1, x_train, y_train, cv= 5)
print("MLPClassifier Cross Validation Attempt 1: "+str(mlpscores))
#[0.98242531 0.99032542 0.98504837 0.9876869  0.98416887]

mlp2 = MLPClassifier(max_iter=8)
mlp2.fit(x_train, y_train)
mlpscores = cross_val_score(mlp2, x_train, y_train, cv= 5)
print("MLPTreeClassifier Cross Validation Attempt 2: "+str(mlpscores))
#max_iter 값을 8까지 늘리면 Cross-Val-Score가 전보다 증가하는 것을 볼 수 있습니다.
#[0.99912127 0.99824099 0.99912049 0.99912049 0.99736148]

cnb1.fit(x_train, y_train)
cnb1.score(x_test, y_test)
cnbscores = cross_val_score(cnb1, x_train, y_train, cv= 5)
print("CategoricalNB Cross Validation Attempt 1: "+str(cnbscores))
#[0.94112478 0.94547054 0.93755497 0.93491645 0.92524186]

cnb2 = CategoricalNB(alpha=0.2)
cnb2.fit(x_train, y_train)
cnbscores = cross_val_score(cnb2, x_train, y_train, cv= 5)
print("CategoricalNB Cross Validation Attempt 2: "+str(cnbscores))
#alpha값을 0.2로 조정했을 때 Cross-Val-Score가 증가하는 것을 볼 수 있습니다.
#[0.95342707 0.96042216 0.95602463 0.95074758 0.93667546]

rfc1.fit(x_train, y_train)
rfcscores = cross_val_score(rfc1, x_train, y_train, cv= 5)
print("RandomForestClassifer Cross Validation Attempt 1: "+str(rfcscores))
#[0.99472759 0.99560246 0.99472296 1.         0.99472296]

rfc2 = RandomForestClassifier(max_depth=9)
rfc2.fit(x_train, y_train)
rfcscores = cross_val_score(rfc2, x_train, y_train, cv= 5)
print("RandomForestClassifer Cross Validation Attempt 2: "+str(rfcscores))
#[1. 1. 1. 1. 1.]
#max_depth 값을 9로 조정했을 때 Cross-Val-Score가 1로 증가하는 것을 볼 수 있습니다.


#Cross Validation 스코어를 판단했을 때, DecisionTreeClassifer가 가장 적합한 모델이라고 결론을 내렸습니다.



kmeans = KMeans(n_clusters=2)
kmeans.fit(x_train)
labels = kmeans.predict(x_train)


a= np.array(y_train)
df = pd.DataFrame({'Labels': labels, 'Actual': a.flatten()})
ct = pd.crosstab(df['Labels'], df['Actual'])
print(ct)



from sklearn.feature_selection import SelectFromModel

select = SelectFromModel(RandomForestClassifier(max_depth=9), threshold='median')

select.fit(x_train, y_train)
x_train_l1 = select.transform(x_train)
print(x_train.shape)
print(x_train_l1.shape)


mask = select.get_support()
plt.matshow(mask.reshape(1, -1), cmap='gray_r')
plt.yticks([0])

#총 특성 117개 중 59개가 선택되었습니다.

dtc3=DecisionTreeClassifier(max_depth=7)
dtc3.fit(x_train_l1, y_train)
dtcscores = cross_val_score(dtc3, x_train, y_train, cv= 5)
print("DecisionTreeClassifier Cross Validation Attempt 3: "+str(dtcscores))

#최종결과 출력
x_test_l1=select.transform(x_test)

dtc3.score(x_test_l1, y_test)
