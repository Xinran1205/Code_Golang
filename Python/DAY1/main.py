# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.

def BinarySearch(arr, val):
    lo = 0
    hi = len(arr)-1
    while lo < hi:
        mid = int(lo+(hi-lo)/2)
        if arr[mid]>=val:
            hi = mid
        else:
            lo = mid+1
    return lo





def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
    print(f'Hi, {name}')  # Press Ctrl+F8 to toggle the breakpoint.


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    print_hi('PyCharm')


arr = [1,2,3,4,5,6,7,8,9,10]
val = 5
print(BinarySearch(arr, val))

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
