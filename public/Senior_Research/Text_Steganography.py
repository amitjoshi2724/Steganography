from PIL import Image
from PIL import ImageMath
import binascii;

def decodeHeader(stegged_dimensions, stegged_pix_val):
    totalBitsAvailable = stegged_dimensions[0]*stegged_dimensions[1]*3;
    exp = 1;
    while(((2**exp) - 1 + exp) < totalBitsAvailable):
        exp += 1
    exp -= 1
    headerLength = exp;
    decoded = ""
    i = 0;
    j = 0;
    while(i < dimensions[0]):
        while(j < dimensions[1]):
            tup = stegged_pix_val[i, j]
            red = str(bin(tup[0]));
            redLSB = red[len(red)-1];
            print(str(redLSB) + " t")
            decoded += redLSB
            if(j < dimensions[1] - 1):
                j += 1
            else:
                j = 0
                i += 1
            headerLength -= 1;
            if(headerLength <= 0):
                return (i, j, decoded)
            green = str(bin(tup[1]));
            greenLSB = green[len(green)-1]
            print(str(greenLSB) + " t")
            decoded += greenLSB
            headerLength -= 1;
            if(headerLength <= 0):
                return (i, j, decoded)
            blue = str(bin(tup[2]));
            blueLSB = blue[len(blue)-1]
            print(str(blueLSB) + " t")
            decoded += blueLSB
            headerLength -= 1;
            if(headerLength <= 0):
                return (i, j, decoded)
        #    j += 1;
        #i += 1
            
                
def encodeHeader(dimensions, pix_val, header):
    i = 0;
    j = 0;
    print ("header in encodeHeader: " + str(header))
    while(i < dimensions[0]):
        while(j < dimensions[1]):
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
                oldI = i;
                oldJ = j;
                if(j < dimensions[1] - 1):
                    j += 1
                else:
                    j = 0
                    i += 1
                print(str(redLSB) + " k")
                red = red[:len(red)-1] + redLSB
                header = header[1:]
                green = str(bin(tup[1]));
                greenLSB = str(header[0])
                print (str(greenLSB) + " k")
                green = green[:len(green)-1] + greenLSB
                header = header[1:]
                blue = str(bin(tup[2]));
                blueLSB = str(header[0])
                print (str(blueLSB) + " k")
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
            except:
                #no more header bits to be encoded
                pix_val[oldI, oldJ] = (int(red, 2), int(green, 2), int(blue, 2))
                return (i, j)
def encode(dimensions, toBeEncoded, pix_val, header):
    (i, j) = encodeHeader(dimensions, pix_val, header);
    print ("i: " + str(i))
    print ("j: " + str(j))
    while(i < dimensions[0]):
        while(j < dimensions[1]):
            tup = pix_val[i, j]
            red = str(bin(tup[0]))
            green = str(bin(tup[1]))
            blue = str(bin(tup[2]))
            try:
                tup = pix_val[i, j]
                red = str(bin(tup[0]));
                redLSB = str(toBeEncoded[0]);
                print(str(redLSB) + " ")
                red = red[:len(red)-1] + redLSB
                toBeEncoded = toBeEncoded[1:]
                green = str(bin(tup[1]));
                greenLSB = str(toBeEncoded[0])
                print (str(greenLSB) + " ")
                green = green[:len(green)-1] + greenLSB
                toBeEncoded = toBeEncoded[1:]
                blue = str(bin(tup[2]));
                blueLSB = toBeEncoded[0]
                print (str(blueLSB) + " ")
                blue = blue[:len(blue)-1] + blueLSB
                toBeEncoded = toBeEncoded[1:]
                pix_val[i, j] = (int(red, 2), int(green, 2), int(blue, 2))
                if(i == 0 and j == 7):
                    print ("in encode [0, 7]: " + str(pix_val[i, j]))
                j += 1
            except:
                # no more bits to be encoded
                pix_val[i, j] = (int(red, 2), int(green, 2), int(blue, 2))
                print ("done encoding")
                return
        i += 1
        j = 0
def decode(stegged_dimensions, stegged_pix_val):
    (i, j, decodedHeader) = decodeHeader(stegged_dimensions, stegged_pix_val);
    print ("decode starti: " + str(i))
    print ("decode startj: " + str(j))
    messageBitLength = int(decodedHeader, 2);
    print ("messageBitLength: " + str(messageBitLength))
    decoded = ""
    while(i < dimensions[0]):
        while(j < dimensions[1]):
            tup = stegged_pix_val[i, j]
            
            red = str(bin(tup[0]));
            redLSB = red[len(red)-1];
            decoded += redLSB
            print (decoded)
            messageBitLength -= 1;
            if(messageBitLength <= 0):
                return decoded
                
            green = str(bin(tup[1]));
            greenLSB = green[len(green)-1]
            decoded += greenLSB
            print (decoded)
            messageBitLength -= 1;
            if(messageBitLength <= 0):
                return decoded
                
            blue = str(bin(tup[2]));
            blueLSB = blue[len(blue)-1]
            decoded += blueLSB
            print (decoded)
            messageBitLength -= 1;
            if(messageBitLength <= 0):
                return decoded
            j += 1;
        i += 1;
        j = 0
            
    return decoded;
            
im = Image.open("IMG_2007.jpg")

encoded = Image.open("IMG_2007.jpg");
pix_val = encoded.load();
dimensions = encoded.size
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
    print("After slicing: " + toBeEncoded)
    print ("Length: " + bin(len(toBeEncoded)))
    print (pix_val[0, 6])
    encode(dimensions, toBeEncoded, pix_val, header)
    print (pix_val[0, 6])
    print ("[0, 7]: " + str(pix_val[0, 7]))
    encoded.save("encoded.png");
    
    # now to retrieve the hiddenmessage
    
    stegged = Image.open("encoded.png");
    stegged_dimensions = stegged.size
    stegged_pix_val = stegged.load();
    print ("stegged: " + str(stegged_pix_val[0, 6]))
    print ("stegged [0, 7]: " + str(stegged_pix_val[0, 7]))
    binaryretrieved = '0b' + decode(stegged_dimensions, stegged_pix_val)
    print ("binaryretrieved: " + str(binaryretrieved))
    n = int(binaryretrieved, 2)
    retrieved = binascii.unhexlify('%x' % n)
    print ("answer: " + str(retrieved))
    #binarystring = ''.join(format(ord(x), 'b') for x in hiddenmessage);
    #print(binarystring);

