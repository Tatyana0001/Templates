#include <string>
#include <iostream>
#include <algorithm>

class Text {
public:
    virtual void render(const std::string& data) const {
        std::cout << data;
    }
};

class DecoratedText : public Text {
public:
    explicit DecoratedText(Text* text) : text_(text) {}
    Text* text_;
};

class ItalicText : public DecoratedText {
public:
    explicit ItalicText(Text* text) : DecoratedText(text) {}
    void render(const std::string& data)  const {
        std::cout << "<i>";
        text_->render(data);
        std::cout << "</i>";
    }
};

class BoldText : public DecoratedText {
public:
    explicit BoldText(Text* text) : DecoratedText(text) {}
    void render(const std::string& data) const {
        std::cout << "<b>";
        text_->render(data);
        std::cout << "</b>";
    }
};

class Paragraph : public DecoratedText {
public:
    explicit Paragraph(Text* text) : DecoratedText(text) {}
    void render(const std::string& data) const {
        std::cout << "<p>";
        text_->render(data);
        std::cout << "</p>";
    }
};

class Reversed : public DecoratedText {
public:
    explicit Reversed(Text* text) : DecoratedText(text) {}
    void render(const std::string& data) const {
        std::string copy(data);
        std::reverse(copy.begin(), copy.end());
        text_->render(copy);
    }
};

class Link : public DecoratedText {
public:
    explicit Link(Text* text) : DecoratedText(text) {}
    void render(const std::string& data, const std::string& data2) const {
        std::cout << "<a href=";
        text_->render(data);
        std::cout << ">";
        text_->render(data2);
        std::cout << "</a>";
    }
};

int main() {
    auto text_block = new ItalicText(new BoldText(new Text()));
    text_block->render("Hello world");
    std::cout << "\n";
    auto text_block1 = new Paragraph(new Text());
    text_block1->render("Hello world");
    std::cout << "\n";
    auto text_block2 = new Reversed(new Text());
    text_block2->render("Hello world");
    std::cout << "\n";
    auto text_block3 = new Link(new Text());
    text_block3->render("netology.ru", "Hello world");
    return 0;
}