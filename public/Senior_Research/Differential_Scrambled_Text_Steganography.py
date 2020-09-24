from PIL import Image
from PIL import ImageMath
import binascii;
import random;

encodedspots = set()

decodedspots = set()
map = dict()
l = list()
    
    
def decodeHeader(stegged_dimensions, stegged_pix_val, decodingList):
    totalBitsAvailable = stegged_dimensions[0]*stegged_dimensions[1]*3;
    exp = 1;
    while(((2**exp) - 1 + exp) < totalBitsAvailable):
        exp += 1
    exp -= 1
    headerLength = exp;
    decoded = ""
    i = 0;
    j = 0;
    prev = '-1';
    while(i < dimensions[0]):
        while(j < dimensions[1]):
            realOriginalI = i
            realOriginalJ = j
            (i, j) = decodingList[(dimensions[0]*i) + j]
            tup = stegged_pix_val[i, j]
            red = str(bin(tup[0]));
            redLSB = red[len(red)-1];
            if(prev == '-1'):
                prev = redLSB;
            else:
                if(redLSB == '0'):
                    redLSB = prev;
                elif(redLSB == '1' and prev == '1'):
                    redLSB = '0';
                    prev = redLSB;
                elif(redLSB == '1' and prev == '0'):
                    redLSB = '1';
                    prev = redLSB;
            print(str(redLSB) + " t")
            decoded += redLSB
            if(j < dimensions[1] - 1):
                realOriginalJ += 1
            else:
                realOriginalJ = 0
                realOriginalI += 1
            headerLength -= 1;
            if(headerLength <= 0):
                i = realOriginalI
                j = realOriginalJ
                return (i, j, decoded)
            green = str(bin(tup[1]));
            greenLSB = green[len(green)-1]
            if(greenLSB == '0'):
                greenLSB = prev
            elif(greenLSB == '1' and prev == '1'):
                greenLSB = '0';
                prev = greenLSB;
            elif(greenLSB == '1' and prev == '0'):
                greenLSB = '1';
                prev = greenLSB;
            print(str(greenLSB) + " t")
            decoded += greenLSB
            headerLength -= 1;
            if(headerLength <= 0):
                i = realOriginalI
                j = realOriginalJ
                return (i, j, decoded)
            blue = str(bin(tup[2]));
            blueLSB = blue[len(blue)-1]
            if(blueLSB == '0'):
                blueLSB = prev
            elif(blueLSB == '1' and prev == '1'):
                blueLSB = '0';
                prev = blueLSB;
            elif(blueLSB == '1' and prev == '0'):
                blueLSB = '1';
                prev = blueLSB;
            print(str(blueLSB) + " t")
            decoded += blueLSB
            headerLength -= 1;
            if(headerLength <= 0):
                i = realOriginalI
                j = realOriginalJ
                return (i, j, decoded)
            i = realOriginalI;
            j = realOriginalJ;
        #    j += 1;
        #i += 1
            
                
def encodeHeader(dimensions, pix_val, header, encodingList):
    i = 0;
    j = 0;
    prev = '-1';
    print ("header in encodeHeader: " + str(header))
    while(i < dimensions[0]):
        while(j < dimensions[1]):
            realOriginalI = i
            realOriginalJ = j
            (i, j) = encodingList[(dimensions[0]*i) + j]
            tup = pix_val[i, j]
            red = str(bin(tup[0]))
            green = str(bin(tup[1]))
            blue = str(bin(tup[2]))
            oldI = i
            oldJ = j
            try:
                tup = pix_val[i, j]
                
                red = str(bin(tup[0]));
                redLSB = str(header[0]);
                prev = redLSB;
                if(prev != '-1'):
                    if(redLSB == '0'):
                        redLSB = prev;
                    else:
                        if(prev == '0'):
                            redLSB = '1'
                        else:
                            redLSB = '0'
                
                oldI = i;
                oldJ = j;
                if(realOriginalJ < dimensions[1] - 1):
                    realOriginalJ += 1
                else:
                    realOriginalJ = 0
                    realOriginalI += 1
                print(str(redLSB) + " kr")
                red = red[:len(red)-1] + redLSB
                header = header[1:]
                green = str(bin(tup[1]));
                greenLSB = str(header[0])
                prev = greenLSB
                if(greenLSB == '0'):
                    greenLSB = prev;
                else:
                    if(prev == '0'):
                        greenLSB = '1'
                    else:
                        #print ("143prev was: " + str(prev))
                        greenLSB = '0'
                        #print ("145greenLSB is now: " + str(greenLSB))
                #print ("146right after, greenLSB is: " + str(greenLSB))
                
                print (str(greenLSB) + " kg")
                green = green[:len(green)-1] + greenLSB
                header = header[1:]
                blue = str(bin(tup[2]));
                blueLSB = str(header[0])
                prev = blueLSB
                if(blueLSB == '0'):
                    blueLSB = prev;
                else:
                    if(prev == '0'):
                        blueLSB = '1'
                    else:
                        blueLSB = '0'
                
                print (str(blueLSB) + " kb")
                blue = blue[:len(blue)-1] + blueLSB
                if(redLSB == '1'):
                    print ("prewoahr: " + bin(tup[0]))
                    print ("woahr: " + str(int(red, 2)))
                if(greenLSB == '1'):
                    print ("prewoahr: " + bin(tup[1]))
                    print ("woahg: " + str(int(green, 2)))
                if(blueLSB == '1'):
                    print ("prewoahr: " + bin(tup[2]))
                    print ("woahb: " + str(int(blue, 2)))
                header = header[1:]
                pix_val[oldI, oldJ] = (int(red, 2), int(green, 2), int(blue, 2))
                i = realOriginalI
                j = realOriginalJ
            except:
                #no more header bits to be encoded
                pix_val[oldI, oldJ] = (int(red, 2), int(green, 2), int(blue, 2))
                i = realOriginalI
                j = realOriginalJ
                return (i, j)
def encode(dimensions, toBeEncoded, pix_val, header, password):
    random.seed(password)
    encodingList = list()
    encodingList = [-1] * (dimensions[0] * dimensions[1]);
    co = 0
    for i in range(dimensions[0]):
        for j in range(dimensions[1]):
            encodingList[(dimensions[0] * j) + i] = (i, j)
            co += 1
    random.shuffle(encodingList)
    print ("co: " + str(co))
    print ("Length: " + str(len(encodingList)))
    #print (encodingList)
    (i, j) = encodeHeader(dimensions, pix_val, header, encodingList);
    print ("i: " + str(i))
    print ("j: " + str(j))
    prev = '-1';
    while(i < dimensions[0]):
        while(j < dimensions[1]):
            
            tup = pix_val[i, j]
            red = str(bin(tup[0]))
            green = str(bin(tup[1]))
            blue = str(bin(tup[2]))
            try:
                realOriginalI = i
                realOriginalJ = j
                (i, j) = encodingList[(dimensions[0]*i) + j]
                tup = pix_val[i, j]
                red = str(bin(tup[0]));
                redLSB = str(toBeEncoded[0]);
                prev = redLSB;
                if(prev != '-1'):
                    if(redLSB == '0'):
                        redLSB = prev;
                    else:
                        if(prev == '0'):
                            redLSB = '1'
                        else:
                            redLSB = '0'
                
                print(str(redLSB) + " ")
                red = red[:len(red)-1] + redLSB
                toBeEncoded = toBeEncoded[1:]
                green = str(bin(tup[1]));
                greenLSB = str(toBeEncoded[0])
                prev = greenLSB
                if(greenLSB == '0'):
                    greenLSB = prev;
                else:
                    if(prev == '0'):
                        greenLSB = '1'
                    else:
                        greenLSB = '0'
                
                print (str(greenLSB) + " ")
                green = green[:len(green)-1] + greenLSB
                toBeEncoded = toBeEncoded[1:]
                blue = str(bin(tup[2]));
                blueLSB = toBeEncoded[0]
                prev = blueLSB
                if(blueLSB == '0'):
                    blueLSB = prev;
                else:
                    if(prev == '0'):
                        blueLSB = '1'
                    else:
                        blueLSB = '0'
                
                print (str(blueLSB) + " ")
                blue = blue[:len(blue)-1] + blueLSB
                toBeEncoded = toBeEncoded[1:]
                pix_val[i, j] = (int(red, 2), int(green, 2), int(blue, 2))
                if(i == 0 and j == 7):
                    print ("in encode [0, 7]: " + str(pix_val[i, j]))
                realOriginalJ += 1
                i = realOriginalI
                j = realOriginalJ
            except:
                # no more bits to be encoded
                pix_val[i, j] = (int(red, 2), int(green, 2), int(blue, 2))
                i = realOriginalI
                j = realOriginalJ
                print ("done encoding")
                return
        i += 1
        j = 0
def decode(stegged_dimensions, stegged_pix_val, potentialpassword):
    random.seed(potentialpassword)
    decodingList = list()
    decodingList = [-1] * (dimensions[0] * dimensions[1]);
    co = 0
    for i in range(dimensions[0]):
        for j in range(dimensions[1]):
            decodingList[(dimensions[0] * j) + i] = (i, j)
            co += 1
    random.shuffle(decodingList)
    print ("co: " + str(co))
    print ("Length: " + str(len(decodingList)))
    (i, j, decodedHeader) = decodeHeader(stegged_dimensions, stegged_pix_val, decodingList);
    print ("decode starti: " + str(i))
    print ("decode startj: " + str(j))
    messageBitLength = int(decodedHeader, 2);
    print ("messageBitLength: " + str(messageBitLength))
    decoded = ""
    prev = '-1';
    while(i < dimensions[0]):
        while(j < dimensions[1]):
            realOriginalI = i
            realOriginalJ = j
            (i, j) = decodingList[(dimensions[0]*i) + j]
            tup = stegged_pix_val[i, j]
            
            red = str(bin(tup[0]));
            redLSB = red[len(red)-1];
            if(prev == '-1'):
                prev = redLSB;
            else:
                if(redLSB == '0'):
                    redLSB = prev;
                elif(redLSB == '1' and prev == '1'):
                    redLSB = '0';
                    prev = redLSB;
                elif(redLSB == '1' and prev == '0'):
                    redLSB = '1';
                    prev = redLSB;
            decoded += redLSB
            #print (decoded)
            messageBitLength -= 1;
            if(messageBitLength <= 0):
                return decoded
                
            green = str(bin(tup[1]));
            greenLSB = green[len(green)-1]
            if(greenLSB == '0'):
                greenLSB = prev
            elif(greenLSB == '1' and prev == '1'):
                greenLSB = '0';
                prev = greenLSB;
            elif(greenLSB == '1' and prev == '0'):
                greenLSB = '1';
                prev = greenLSB;
            decoded += greenLSB
            #print (decoded)
            messageBitLength -= 1;
            if(messageBitLength <= 0):
                return decoded
                
            blue = str(bin(tup[2]));
            blueLSB = blue[len(blue)-1]
            if(blueLSB == '0'):
                blueLSB = prev
            elif(blueLSB == '1' and prev == '1'):
                blueLSB = '0';
                prev = blueLSB;
            elif(blueLSB == '1' and prev == '0'):
                blueLSB = '1';
                prev = blueLSB;
            decoded += blueLSB
            #print (decoded)
            messageBitLength -= 1;
            if(messageBitLength <= 0):
                return decoded
                
            realOriginalJ += 1;
            i = realOriginalI
            j = realOriginalJ
        i += 1;
        j = 0
            
    return decoded;
            
im = Image.open("IMG_2007.jpg")

encoded = Image.open("IMG_2007.jpg");
pix_val = encoded.load();
dimensions = encoded.size
print ("dimensions: " + str(dimensions))

random.shuffle(l)
totalBitsAvailable = dimensions[0]*dimensions[1]*3;
print ("totalBitsAvailable: " + str(totalBitsAvailable))
hiddenmessage = raw_input("Enter message you want to encode: ")
binarystring = bin(int(binascii.hexlify(hiddenmessage), 16))
print ("before slicing: " + binarystring)
toBeEncoded = binarystring[2:]
hiddenMessageLength = len(toBeEncoded)
header = bin(hiddenMessageLength)[2:]
print ("header: " + header)
headerLength = len(header)
maxHeaderLength = -1;
exp = 1;
while(((2**exp) - 1 + exp) < totalBitsAvailable):
    exp += 1
exp -= 1
maxHeaderLength = exp;
print ("maxHeaderLength: " + str(maxHeaderLength))
for i in range(maxHeaderLength - headerLength):
    header = "0" + header
headerLength = len(header)
print ("header: " + header)
if(headerLength + hiddenMessageLength > totalBitsAvailable):
    print ("Error: trying to encode too much data for this image\tTotal bits available: " + str(totalBitsAvailable) +
    "\t" + "trying to encode: " + str(hiddenMessageLength+headerLength) + " bits.")
else:
    password = str(raw_input("Enter password which you should write down: "))
    print("After slicing: " + toBeEncoded)
    print ("Length: " + bin(len(toBeEncoded)))
    print (pix_val[0, 6])
    encode(dimensions, toBeEncoded, pix_val, header, password)
    print (pix_val[0, 6])
    print ("[0, 7]: " + str(pix_val[0, 7]))
    encoded.save("encoded.png");
    
    # now to retrieve the hiddenmessage
    
    potentialpassword = str(raw_input("Enter password to decode the thing: "))
    stegged = Image.open("encoded.png");
    stegged_dimensions = stegged.size
    stegged_pix_val = stegged.load();
    print ("stegged: " + str(stegged_pix_val[0, 6]))
    print ("stegged [0, 7]: " + str(stegged_pix_val[0, 7]))
    binaryretrieved = '0b' + decode(stegged_dimensions, stegged_pix_val, potentialpassword)
    print ("binaryretrieved: " + str(binaryretrieved))
    n = int(binaryretrieved, 2)
    retrieved = binascii.unhexlify('%x' % n)
    print ("answer: " + str(retrieved))
    #binarystring = ''.join(format(ord(x), 'b') for x in hiddenmessage);
    #print(binarystring);