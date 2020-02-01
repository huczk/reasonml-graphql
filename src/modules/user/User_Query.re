type t = {
  id: string,
  username: string,
  email: string,
  image: string,
  bio: string,
  token: option(string),
};

module User = [%graphql
  {|
    query user {
      user @bsRecord {
        id
        token
        username
        email
        image
        bio
      }
    }
  |}
];

module UserFragment = [%graphql
  {|
    fragment User on User {
      id
      token
      username
      email
      image
      bio
    }
  |}
];

module Login = [%graphql
  {|
    mutation login($email: String!, $password: String!) {
      login(input: { email: $email, password: $password }) {
        ...UserFragment.User
      }
    }
  |}
];

module Register = [%graphql
  {|
    mutation register($email: String!, $password: String!, $username: String!) {
      register(input: { email: $email, password: $password, username: $username }) {
        ...UserFragment.User
      }
    }
  |}
];

module UpdateProfile = [%graphql
  {|
    mutation updateProfile($image: String, $username: String $bio: String, $email: String, $password: String) {
      updateProfile(input: { image: $image, username: $username, bio: $bio, email: $email, password: $password }) {
        ...UserFragment.User
      }
    }
  |}
];