import tensorflow as tf
import random
import loadData
import handCNN


def save_model(saver, sess, save_path):
    path = saver.save(sess, save_path)


if __name__ == '__main__':
    sess = tf.InteractiveSession()
    y_conv, rmse = handCNN.model()
    train_step = tf.train.AdamOptimizer(1e-3).minimize(rmse)

    #init all variables
    sess.run(tf.initialize_all_variables())
    X,y = loadData.input_data()
    X_valid, y_valid = X[:loadData.VALI_SIZE], y[:loadData.VALI_SIZE]
    X_train, y_train = X[loadData.VALI_SIZE:], y[loadData.VALI_SIZE:]

    best_validation_loss = 1000000.0
    current_epoch = 0
    TRAIN_SIZE = X_train.shape[0]
    train_index = range(TRAIN_SIZE)
    random.shuffle(train_index)
    X_train, y_train = X_train[train_index], y_train[train_index]

    saver = tf.train.Saver()

    print 'training*---'
    for i in xrange(loadData.EPOCHS):
        random.shuffle(train_index)  
        X_train, y_train = X_train[train_index], y_train[train_index]

        for j in xrange(0, TRAIN_SIZE, loadData.BATCH_SIZE):
            print 'epoch {0}, train {1} samples done...'.format(i, j)

            train_step.run(feed_dict={handCNN.x: X_train[j:j + loadData.BATCH_SIZE],
                                      handCNN.y_: y_train[j:j + loadData.BATCH_SIZE], handCNN.keep_prob: 0.5})


        train_loss = rmse.eval(feed_dict={x:X_train, y_:y_train, keep_prob: 1.0})
        validation_loss = rmse.eval(feed_dict={handCNN.x: X_valid, handCNN.y_: y_valid, handCNN.keep_prob: 1.0})

        print 'fineshed epoch {0} with validation loss:{1}'.format(i, validation_loss * 96.0)
        if validation_loss < best_validation_loss:
            best_validation_loss = validation_loss
            current_epoch = i
            save_model(saver, sess, loadData.SAVE_PATH)  
        elif (i - current_epoch) >= loadData.EARLY_STOP:
            print 'early stop applied'
            break

