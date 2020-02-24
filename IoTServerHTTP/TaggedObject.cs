using System.Collections.Generic;

public class TaggedObject {
    static public TaggedObject DIAMOND = new TaggedObject(87, 43, 140, 245, NeoPixelColor.Aqua, NeoPixelColor.Grey);
    static public TaggedObject CANDY_CANE = new TaggedObject(23, 180, 138, 245, NeoPixelColor.Red, NeoPixelColor.White);
    static public TaggedObject SKULL = new TaggedObject(119, 180, 138, 245, NeoPixelColor.Brown, NeoPixelColor.Grey);
    static public TaggedObject WIZARD_HAT = new TaggedObject(23, 6, 138, 245, NeoPixelColor.Purple, NeoPixelColor.Yellow);
    static public TaggedObject FISH = new TaggedObject(247, 42, 140, 245, NeoPixelColor.Orange, NeoPixelColor.Aqua);
    static public TaggedObject GEARS = new TaggedObject(215, 181, 139, 245, NeoPixelColor.Yellow, NeoPixelColor.Orange);
    static public TaggedObject FLOWER = new TaggedObject(55, 182, 139, 245, NeoPixelColor.Green, NeoPixelColor.Pink);
    static public TaggedObject MUSHROOM = new TaggedObject(119, 6, 138, 245, NeoPixelColor.Brown, NeoPixelColor.Orange);

    static public TaggedObject CARD_1_CLOCK = new TaggedObject(23, 181, 139, 245, 
        NeoPixelColor.Yellow, NeoPixelColor.Grey, NeoPixelColor.Grey, NeoPixelColor.Grey, NeoPixelColor.Grey, NeoPixelColor.Grey);
    static public TaggedObject CARD_2_PENS = new TaggedObject(119, 181, 139, 245, 
        NeoPixelColor.Yellow, NeoPixelColor.Grey, NeoPixelColor.Grey, NeoPixelColor.Yellow, NeoPixelColor.Grey, NeoPixelColor.Grey);
    static public TaggedObject CARD_3_CANDLES = new TaggedObject(135, 42, 140, 245, 
        NeoPixelColor.Yellow, NeoPixelColor.Grey, NeoPixelColor.Yellow, NeoPixelColor.Grey, NeoPixelColor.Yellow, NeoPixelColor.Grey);
    static public TaggedObject CARD_4_FAERIES = new TaggedObject(215, 214, 140, 245, 
        NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Grey, NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Grey);
    static public TaggedObject CARD_5_STARS = new TaggedObject(215, 44, 141, 245, 
        NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Grey);
    static public TaggedObject CARD_6_BOOKS = new TaggedObject(55, 215, 140, 245, 
        NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Yellow);

    static public TaggedObject DRAGON_LIGHTNING = new TaggedObject(162, 87, 26, 39, NeoPixelColor.Yellow, NeoPixelColor.Aqua);
    static public TaggedObject DRAGON_ACID = new TaggedObject(208, 8, 205, 37, NeoPixelColor.Yellow, NeoPixelColor.Green);
    static public TaggedObject DRAGON_SHRIEKING = new TaggedObject(192, 200, 16, 37, NeoPixelColor.Grey, NeoPixelColor.Purple);
    static public TaggedObject DRAGON_RUBY = new TaggedObject(113, 161, 84, 27, NeoPixelColor.Red, NeoPixelColor.Brown);
    static public TaggedObject DRAGON_STONE = new TaggedObject(192, 198, 58, 37, NeoPixelColor.Grey, NeoPixelColor.White);
    static public TaggedObject DRAGON_WATER = new TaggedObject(242, 10, 0, 210, NeoPixelColor.Purple, NeoPixelColor.Aqua);
    static public TaggedObject DRAGON_FIRE = new TaggedObject(246, 37, 23, 37, NeoPixelColor.Red, NeoPixelColor.Yellow);
    static public TaggedObject DRAGON_CRYSTAL = new TaggedObject(208, 153, 194, 37, NeoPixelColor.Blue, NeoPixelColor.Aqua);

    static public TaggedObject GEM_YOUTH = new TaggedObject(103, 215, 140, 245, 
        NeoPixelColor.Green, NeoPixelColor.Blue, NeoPixelColor.Purple, NeoPixelColor.Red, NeoPixelColor.Purple, NeoPixelColor.Blue);
    static public TaggedObject GEM_SPRING = new TaggedObject(39, 249, 133, 245, 
        NeoPixelColor.Blue, NeoPixelColor.Purple, NeoPixelColor.Red, NeoPixelColor.Orange, NeoPixelColor.Red, NeoPixelColor.Purple);
    static public TaggedObject GEM_GOLD = new TaggedObject(199, 166, 130, 245, 
        NeoPixelColor.Purple, NeoPixelColor.Red, NeoPixelColor.Orange, NeoPixelColor.Yellow, NeoPixelColor.Orange, NeoPixelColor.Red);
    static public TaggedObject GEM_LEAD = new TaggedObject(71, 223, 129, 245, 
        NeoPixelColor.Red, NeoPixelColor.Orange, NeoPixelColor.Yellow, NeoPixelColor.Green, NeoPixelColor.Yellow, NeoPixelColor.Orange);
    static public TaggedObject GEM_ADULT = new TaggedObject(199, 215, 140, 245, 
        NeoPixelColor.Orange, NeoPixelColor.Yellow, NeoPixelColor.Green, NeoPixelColor.Blue, NeoPixelColor.Green, NeoPixelColor.Yellow);
    static public TaggedObject GEM_WINTER = new TaggedObject(199, 248, 133, 245, 
        NeoPixelColor.Yellow, NeoPixelColor.Green, NeoPixelColor.Blue, NeoPixelColor.Purple, NeoPixelColor.Blue, NeoPixelColor.Green);

    static public TaggedObject BOTTLE_YELLOW = new TaggedObject(71, 25, 128, 245, NeoPixelColor.Yellow, NeoPixelColor.Yellow);
    static public TaggedObject BOTTLE_GREEN = new TaggedObject(151, 24, 128, 245, NeoPixelColor.Green, NeoPixelColor.Green);
    static public TaggedObject BOTTLE_BLUE = new TaggedObject(39, 19, 128, 245, NeoPixelColor.Blue, NeoPixelColor.Blue);
    static public TaggedObject BOTTLE_MAGENTA = new TaggedObject(71, 190, 128, 245, NeoPixelColor.Pink, NeoPixelColor.Pink);
    static public TaggedObject BOTTLE_RED = new TaggedObject(231, 189, 128, 245, NeoPixelColor.Red, NeoPixelColor.Red);
    static public TaggedObject BOTTLE_TEAL = new TaggedObject(183, 179, 138, 245, NeoPixelColor.Aqua, NeoPixelColor.Aqua);
    static public TaggedObject BOTTLE_ORANGE = new TaggedObject(199, 20, 128, 245, NeoPixelColor.Orange, NeoPixelColor.Orange);
    static public TaggedObject BOTTLE_VIOLET = new TaggedObject(135, 39, 127, 245, NeoPixelColor.Purple, NeoPixelColor.Purple);

    static public TaggedObject COIN_A = new TaggedObject(231, 5, 138, 245, NeoPixelColor.Yellow, NeoPixelColor.Orange);
    static public TaggedObject COIN_B = new TaggedObject(23, 179, 138, 245, NeoPixelColor.Yellow, NeoPixelColor.Orange);
    static public TaggedObject COIN_C = new TaggedObject(39, 39, 127, 245, NeoPixelColor.Yellow, NeoPixelColor.Orange);
    static public TaggedObject COIN_D = new TaggedObject(151, 180, 126, 245, NeoPixelColor.Yellow, NeoPixelColor.Orange);
    static public TaggedObject COIN_E = new TaggedObject(119, 179, 138, 245, NeoPixelColor.Yellow, NeoPixelColor.Orange);

    static public TaggedObject COMPASS = new TaggedObject(39, 193, 135, 245, 
        NeoPixelColor.Red, NeoPixelColor.White, NeoPixelColor.White, NeoPixelColor.Red, NeoPixelColor.White, NeoPixelColor.White);
    static public TaggedObject AMULET = new TaggedObject(71, 218, 136, 245, NeoPixelColor.Brown, NeoPixelColor.Yellow);

    static private List<TaggedObject> allObjects;

    private int[] cardData;

    public NeoPixelColor[] colors {get; private set;}

    public TaggedObject(int a, int b, int c, int d, NeoPixelColor color1, NeoPixelColor color2) {

        cardData = new int[4] { a, b, c, d };
        colors = new NeoPixelColor[6] { color1, color2, color1, color2, color1, color2 };

        if (allObjects == null) {
            allObjects = new List<TaggedObject>();
        }
        allObjects.Add(this);
    }

    public TaggedObject(int a, int b, int c, int d, 
        NeoPixelColor color1, NeoPixelColor color2, NeoPixelColor color3,
        NeoPixelColor color4, NeoPixelColor color5, NeoPixelColor color6) {

        cardData = new int[4] { a, b, c, d };
        colors = new NeoPixelColor[6] { color1, color2, color3, color4, color5, color6 };

        if (allObjects == null) {
            allObjects = new List<TaggedObject>();
        }
        allObjects.Add(this);
    }

    public bool matches(CardReaderData card) {
        for (int i = 0; i < 4; ++i) {
            if (card.cardData[i] != cardData[i]) {
                return false;
            }
        }
        return true;
    }

    static public TaggedObject FindObject(CardReaderData card) {
        foreach(var o in allObjects) {
            if (o.matches(card)) {
                return o;
            }
        }
        return null;
    }
}