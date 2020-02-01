module GetProfile = [%graphql
  {|
    query getProfile($username: String!) {
      profile(username: $username) {
        id
        username
        bio
        image
        following
      }
    }
  |}
];

module Follow = [%graphql
  {|
    mutation follow($username: String!, $follow: Boolean!) {
      follow(username: $username, follow: $follow) {
        id
        following
      }
    }
  |}
];